cmake_minimum_required(VERSION 3.17)
project(pp_tp1)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_FLAGS "-Wall -Wextra -pthread")
set(CMAKE_CXX_FLAGS_RELEASE "-O3")


add_executable(pp_tp1 main.cpp ArgParse.cpp ArgParse.hpp MillerRabinSeq.cpp MillerRabinSeq.hpp MillerRabinPar.cpp MillerRabinPar.hpp FileParseMpz.cpp FileParseMpz.hpp)
target_link_libraries(pp_tp1 gmp gmpxx pthread)