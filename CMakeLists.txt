cmake_minimum_required(VERSION 3.16)
project(Pyro CXX)

file(GLOB_RECURSE SRC "lib/*.cpp")
include_directories(include)

# include rapidcsv
include_directories(rapidcsv/src)
<<<<<<< Updated upstream

# include boost
find_package(boost REQUIRED)
include_directories(${Boost_INCLUDE_DIRS})

add_compile_options(-Wall -Wextra -g -fexceptions -std=c++17)
add_link_options(-lpthread)
=======
add_compile_options(-Ofast -Wall -Wextra -g -fexceptions -std=c++17 -lpthread)
>>>>>>> Stashed changes
add_library(Pyro Pyro.cpp ${SRC})
