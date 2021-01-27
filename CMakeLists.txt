cmake_minimum_required(VERSION 3.17)
project(pp_tp1)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_FLAGS "-Wall -Wextra")
set(CMAKE_CXX_FLAGS_RELEASE "-O3")

add_executable(pp_tp1 main.cpp FileParse.cpp FileParse.hpp ArgParse.cpp ArgParse.hpp)