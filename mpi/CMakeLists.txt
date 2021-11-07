cmake_minimum_required(VERSION 3.20)
project(mpi)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_FLAGS_DEBUG "-g")
set(CMAKE_CXX_FLAGS_RELEASE "-O3")

find_package(MPI REQUIRED)
add_definitions(-DOMPI_SKIP_MPICXX)

add_executable(cell_automata cell_automata.cpp)
target_link_libraries(cell_automata
        PRIVATE
        MPI::MPI_C)

add_executable(meeting meeting.cpp)
target_link_libraries(meeting
        PRIVATE
        MPI::MPI_C)
