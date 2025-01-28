cmake_minimum_required(VERSION 3.30)
project(Shell_Elbalad)

set(CMAKE_CXX_STANDARD 20)

find_package(Curses REQUIRED)
include_directories(${CURSES_INCLUDE_DIRS})

add_executable(Shell_Elbalad main.cpp command.h general_commands.h helper_functions.h data.h history.h)

target_link_libraries(Shell_Elbalad ${CURSES_LIBRARIES})