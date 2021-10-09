cmake_minimum_required(VERSION 3.18)
project(matrix_multiplication)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_FLAGS_DEBUG "-g")
set(CMAKE_CXX_FLAGS_RELEASE "-O3")

add_library(matrix STATIC matrix.cpp)
add_library(vector STATIC vector.cpp)
add_executable(matrix_multiplication main.cpp)
target_link_libraries(matrix_multiplication matrix vector)
