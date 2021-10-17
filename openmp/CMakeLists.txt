cmake_minimum_required(VERSION 3.20)
project(openmp)

set(CMAKE_CXX_STANDARD 17)

if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Release)
endif()

set(CMAKE_CXX_FLAGS "-Wall -Wextra")
set(CMAKE_CXX_FLAGS_DEBUG "-g")
set(CMAKE_CXX_FLAGS_RELEASE "-O3")

find_package(OpenMP REQUIRED)
if (OPENMP_FOUND)
    set (CMAKE_C_FLAGS "${CMAKE_C_FLAGS} ${OpenMP_C_FLAGS}")
    set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${OpenMP_CXX_FLAGS}")
endif()

add_executable(omp_bugparfoc omp_bugparfoc.c)
target_link_libraries(omp_bugparfoc PRIVATE OpenMP::OpenMP_CXX)

add_executable(omp_bugreduction omp_bugreduction.c)
target_link_libraries(omp_bugreduction PRIVATE OpenMP::OpenMP_CXX)

add_executable(omp_bugreduction_2 omp_bugreduction_2.c)
target_link_libraries(omp_bugreduction_2 PRIVATE OpenMP::OpenMP_CXX)

add_executable(omp_hello omp_hello.c)
target_link_libraries(omp_hello PRIVATE OpenMP::OpenMP_CXX)

add_executable(omp_sumarray omp_sumarray.c)
target_link_libraries(omp_sumarray PRIVATE OpenMP::OpenMP_CXX)

add_executable(omp_outmes omp_outmes.c)
target_link_libraries(omp_outmes PRIVATE OpenMP::OpenMP_CXX)

add_executable(omp_parsec omp_parsec.c)
target_link_libraries(omp_parsec PRIVATE OpenMP::OpenMP_CXX)

add_executable(omp_privateshared omp_privateshared.c)
target_link_libraries(omp_privateshared PRIVATE OpenMP::OpenMP_CXX)

add_executable(omp_pi omp_pi.cpp)
target_link_libraries(omp_pi PRIVATE OpenMP::OpenMP_CXX)
