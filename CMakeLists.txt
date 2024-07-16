cmake_minimum_required(VERSION 3.13)
project(ouroboros)

set(CMAKE_CXX_STANDARD 17)

# The 'SYSTEM' silences compiler about boost warnings
find_package(Boost CONFIG)
include_directories(SYSTEM ${Boost_INCLUDE_DIRS})

add_executable(ouroboros ouroboros.cpp)
