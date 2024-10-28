cmake_minimum_required(VERSION 3.5)
project(commandline-parser)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

include_directories(SYSTEM ${CMAKE_CURRENT_BINARY_DIR})
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/include)

set(EXECUTABLE_OUTPUT_PATH ${PROJECT_BINARY_DIR}/bin)
set(LIBRARY_OUTPUT_PATH ${PROJECT_BINARY_DIR}/lib)

add_library(CommandLineParser OBJECT src/CommandLineParser.cpp)

add_executable(commandline-parser src/main.cpp $<TARGET_OBJECTS:CommandLineParser>)

target_link_libraries(commandline-parser PRIVATE)