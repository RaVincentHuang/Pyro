cmake_minimum_required(VERSION 3.16)
project(Pyro CXX)

file(GLOB_RECURSE SRC "lib/*.cpp")
include_directories(include)

find_package(Boost REQUIRED)
include_directories(${Boost_INCLUDE_DIRS})

add_link_options(-lpthread)

# include rapidcsv
include_directories(rapidcsv/src)
add_compile_options(-Ofast -Wall -Wextra -g -fexceptions -std=c++20 -lpthread)
add_library(Pyro Pyro.cpp ${SRC})
