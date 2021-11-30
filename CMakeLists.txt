cmake_minimum_required(VERSION 3.16)

project(warships)

set(CMAKE_CXX_STANDARD 14)

include_directories(include)

add_executable(warships
        include/bonus.h
        include/field.h
        include/game.h
        include/gamerules.h
        include/parser.h
        include/ship.h
        include/standards.h
        include/ui.h
        include/user.h
        src/bonus.cpp
        src/field.cpp
        src/game.cpp
        src/gamerules.cpp
        src/parser.cpp
        src/user.cpp
        src/ui.cpp
        src/main.cpp)
