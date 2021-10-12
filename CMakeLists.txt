cmake_minimum_required(VERSION 3.16)

project(warships)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_BUILD_TYPE Debug)

include_directories(include)

add_executable(warships
        src/bonus.cpp
        src/field.cpp
        src/game.cpp
        src/gamerules.cpp
        src/parser.cpp
        src/user.cpp
        src/ui.cpp
        src/main.cpp)
