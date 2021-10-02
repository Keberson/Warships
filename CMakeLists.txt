cmake_minimum_required(VERSION 3.16)

project(warships)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_BUILD_TYPE Debug)

include_directories(include)

add_executable(warships
        include/standards.h
        include/game.h
        include/user.h
        include/gamerules.h
        include/field.h
        src/game.cpp
        src/gamerules.cpp
        src/field.cpp
        src/user.cpp
        src/main.cpp)
