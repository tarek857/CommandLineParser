# CommandLineParser

CommandLineParser is a flexible and easy-to-use command line parser written in C++. It allows the creation and parsing of command line options, supporting various data types. This tool is designed to simplify the process of handling command line arguments in applications, enhancing both usability and maintenance.

### Features
- Supports parsing of string, boolean, and integer pointer types.
- Utilizes modern C++ features like smart pointers and std::variant to manage dynamic types.
- Customizable help generation based on provided command line options.

### Requirements
- C++17
- CMake (version 3.5 or higher)

### Building the Project
To build the CommandLineParser project, you can use the provided CMakeLists.txt file. Below are the steps to compile the project:

```bash
./build.sh
```

## Usage Example
Below is an example of how to use CommandLineParser in your C++ application:

- First, include the CommandLineParser.h in your application:

```cpp
#include "CommandLineParser.h"

int main(int argc, char* argv[]) {
    CommandLineParser parser("Example usage of the CommandLineParser");

    int intValue;
    bool boolValue;
    std::string stringValue;

    // Add options
    parser.addOption({"-i", "--integer"}, &intValue, "Set an integer value");
    parser.addOption({"-b", "--boolean"}, &boolValue, "Set a boolean value");
    parser.addOption({"-s", "--string"}, &stringValue, "Set a string value");

    // Parse command line
    parser.parse(std::vector<std::string_view>(argv + 1, argv + argc));

    // Utilize the parsed values
    std::cout << "Integer: " << intValue << "\n";
    std::cout << "Boolean: " << (boolValue ? "true" : "false") << "\n";
    std::cout << "String: " << stringValue << "\n";

    return 0;
}

```
Compiling and Running
Compile your application using the provided build.sh script or a similar build command. Run the application with different command line options to see how the parser behaves.

```bash
./commandline-parser -i 42 -b -s "Hello, world!"
```