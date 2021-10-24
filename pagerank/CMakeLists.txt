cmake_minimum_required(VERSION 3.20)
project(pagerank)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_FLAGS_DEBUG "-g")
set(CMAKE_CXX_FLAGS_RELEASE "-O3")

find_package(OpenMP REQUIRED)
if (OPENMP_FOUND)
    set (CMAKE_C_FLAGS "${CMAKE_C_FLAGS} ${OpenMP_C_FLAGS}")
    set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${OpenMP_CXX_FLAGS}")
endif()

find_package(OpenBLAS REQUIRED)

add_library(matrix STATIC matrix.cpp)
add_library(vector STATIC vector.cpp)
target_link_libraries(matrix PRIVATE OpenMP::OpenMP_CXX ${OpenBLAS_LIBRARIES})
target_link_libraries(vector PRIVATE OpenMP::OpenMP_CXX ${OpenBLAS_LIBRARIES})

add_executable(number_of_paths number_of_paths.cpp)
target_link_libraries(number_of_paths PRIVATE OpenMP::OpenMP_CXX matrix vector ${OpenBLAS_LIBRARIES})

add_executable(pagerank pagerank.cpp)
target_link_libraries(pagerank PRIVATE OpenMP::OpenMP_CXX matrix vector ${OpenBLAS_LIBRARIES})
