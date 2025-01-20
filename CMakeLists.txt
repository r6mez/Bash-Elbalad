cmake_minimum_required(VERSION 3.30)
project(Shell_Elbalad)

set(CMAKE_CXX_STANDARD 20)

add_executable(Shell_Elbalad main.cpp command.h general_commands.h helper_functions.h data.h)
