cmake_minimum_required(VERSION 3.16)
project(Pyro CXX)

file(GLOB_RECURSE SRC "lib/*.cpp")
include_directories(include)
add_compile_options(-Wall -Wextra -g -fexceptions -std=c++17 -lpthread)
add_library(Pyro Pyro.cpp ${SRC})
