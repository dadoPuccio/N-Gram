cmake_minimum_required(VERSION 3.23)
project(n_gram)

set(CMAKE_CXX_STANDARD 14)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fopenmp ")

add_executable(n_gram main.cpp)
