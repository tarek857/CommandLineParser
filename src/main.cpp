#include "CommandLineParser.h"

#include <iostream>

static void parseCommandLineOptions(const std::vector<std::string_view> &arguments)
{
    CommandLineParser parser("Progrma to parse command-line arguments \nauthor : Tarek sh");

    bool help = false;
    bool debug = true;
    std::string command;

    using Option = CommandLineParser::Option;

    parser.addOption(Option({"-d", "--debug"}, &debug, "set debug level"));
    parser.addOption(Option{{"-h", "--help"}, &help, "Prints help information."});

    parser.parse(arguments);

    if (help)
    {
        parser.printHelp();
        exit(EXIT_SUCCESS);
    }
}

int main(int argc, char **argv)
{
    std::vector<std::string_view> args(argv + 1, argv + argc);
    parseCommandLineOptions(args);

    return 0;
}