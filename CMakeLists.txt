cmake_minimum_required(VERSION 3.15)
project(EnumHelpersExample)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED)

cmake_policy(SET CMP0079 NEW)

include(CMakeParseArguments)

add_executable(EnumHelpersExample Example.cpp)