#include <iostream>
#include <cstdint>
#include <string>
#include <variant>
#include <vector>

class CommandLineParser
{
  public:
    class Option  
    {
      public:
        using integerVariant = std::variant<int *, uint32_t *>;
        using SupportedValues = std::variant<std::string *, bool *, integerVariant>;

        explicit Option(std::vector<std::string> flags, SupportedValues value, std::string help) noexcept
            : _flags(std::move(flags)), _value(std::move(value)), _help(std::move(help))
        {
        }

        std::vector<std::string> const &flags() const noexcept { return _flags; }

        SupportedValues value() const noexcept { return _value; }

        std::string help() const noexcept { return _help; }

        Option(const Option &) = delete;
        Option &operator=(const Option &) = delete;
        Option(Option &&other) noexcept : _flags(std::move(other._flags)), _value(std::move(other._value)), _help(std::move(other._help)) {}

        ~Option() = default;

      private:
        std::vector<std::string> _flags;
        SupportedValues _value;
        std::string _help;
    };

    explicit CommandLineParser(std::string_view description) : _description(description) {}

    void addOption(Option option) { _arguments.emplace_back(std::move(option)); }

    void parse(const std::vector<std::string_view> &argumenetList) const;

    void printHelp(std::ostream &os = std::cout) const;

  private:
    bool hanldleArgument(const Option &argument, const std::string &value, std::string_view flag, bool &valueIsSeparated) const;

    const std::string_view _description;
    std::vector<Option> _arguments;
};