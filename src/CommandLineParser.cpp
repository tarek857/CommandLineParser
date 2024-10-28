#include <CommandLineParser.h>

#include <algorithm>
#include <functional>
#include <iomanip>
#include <numeric>
#include <optional>
#include <sstream>

template <class... Ts>
struct overload : Ts...
{
    using Ts::operator()...;
};
template <class... Ts>
overload(Ts...) -> overload<Ts...>;

template <class T>
static std::optional<T> stringToInteger(std::string_view stringToConvert)
{
    try
    {
        if constexpr (std::is_same_v<T, int>)
        {
            return std::stoi(stringToConvert.data());
        }
        else if constexpr (std::is_same_v<T, uint32_t>)
        {
            return static_cast<uint32_t>(std::stoull(stringToConvert.data()));
        }
    }
    catch (const std::invalid_argument &e)
    {
        std::cerr << "Invalid argument: " << e.what() << std::endl;
    }
    catch (const std::out_of_range &e)
    {
        std::cerr << "Out of range: " << e.what() << std::endl;
    }
    return {};
}

static void handleVariantInteger(CommandLineParser::Option::integerVariant var, std::string_view value, std::string_view flag)
{
    std::visit(
        overload{[&value, flag](int *arg)
                 {
                     if (const auto result = stringToInteger<int>(value); !result)
                     {
                         std::cerr << "The value '" << value << "' of '" << flag << "' could no be converted to an integer, default will be taked.\n";
                     }
                     else
                     {
                         *arg = *result;
                     }
                 },
                 [&value, flag](uint32_t *arg)
                 {
                     if (const auto result = stringToInteger<uint32_t>(value); !result)
                     {
                         std::cerr << "The value[" << value << "] of [" << flag << "] could no be converted to an integer, default will be taked.\n";
                     }
                     else
                     {
                         *arg = *result;
                     }
                 }},
        var);
}

static int getMaxFlagLength(const std::vector<CommandLineParser::Option> &arguments, const int offset)
{
    int maxFlagLength = 0;

    for (auto const &argument : arguments)
    {
        const auto flagLength = std::accumulate(argument.flags().begin(), argument.flags().end(), 0,
                                                [](int sum, const std::string_view &elem) { return sum + static_cast<int>(elem.size()); }) +
                                offset;
        maxFlagLength = std::max(maxFlagLength, flagLength);
    }
    return maxFlagLength;
}

void CommandLineParser::printHelp(std::ostream &os) const
{
    os << _description << std::endl;
    int maxFlagLength = getMaxFlagLength(_arguments, 3);

    for (auto const &argument : _arguments)
    {
        std::string flags;
        for (size_t i = 0; i < argument.flags().size(); i++)
        {
            // don't add the comma at tha end of flags
            if (i != argument.flags().size() - 1)
            {
                flags += argument.flags()[i] + ", ";
            }
            else
            {
                flags += argument.flags()[i];
            }
        }
        std::stringstream sstr;
        sstr << std::left << std::setw(maxFlagLength) << flags;  // NOSONAR change with std::format when we have gcc-13
        sstr << argument.help();
        os << sstr.str() << std::endl;
    }
}

bool CommandLineParser::hanldleArgument(const Option &argument, const std::string &value, std::string_view flag, bool &valueIsSeparated) const
{
    if (std::find(argument.flags().begin(), argument.flags().end(), flag) != argument.flags().end())
    {
        std::visit(overload{[&value, flag](CommandLineParser::Option::integerVariant arg) { handleVariantInteger(arg, value, flag); },
                            [&value, &valueIsSeparated](bool *arg)
                            {
                                if (!value.empty() && value != "true" && value != "false")
                                {
                                    valueIsSeparated = false;
                                }
                                *arg = value != "false";
                            },
                            [&value](std::string *arg) { *arg = value; }},
                   argument.value());
        return true;
    }
    return false;
}

void CommandLineParser::parse(const std::vector<std::string_view> &argumenetList) const
{
    // iterate over the flags
    for (size_t i = 0; i < argumenetList.size();)
    {
        auto flag = std::string(argumenetList[i]);
        std::string value;
        bool valueIsSeparated = false;

        if (const size_t equalPos = flag.find('='); equalPos != std::string::npos)
        {
            value = flag.substr(equalPos + 1);
            flag = flag.substr(0, equalPos);
        }
        else if (i + 1 < argumenetList.size())
        {
            value = argumenetList[i + 1];
            valueIsSeparated = true;
        }

        bool argumentFound = false;
        for (auto const &argument : _arguments)
        {
            if (argumentFound = hanldleArgument(argument, value, flag, valueIsSeparated); argumentFound)
            {
                break;
            }
        }

        if (!argumentFound)
        {
            std::cerr << "Flag \"" << flag << "\"is not recognized.ignore it" << std::endl;
        }

        // if value is sperated or boolean without value jump one index more
        i = i + (argumentFound && valueIsSeparated ? 2 : 1);
    }
}