cmake_minimum_required(VERSION 3.10)

# set the project name
project(matrix_chain_multiplication)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

add_compile_options(-Wall -Wextra -pedantic -Werror)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "bin")

add_subdirectory(matrix)
add_subdirectory(tests)
add_subdirectory(benchmarks)

add_executable(print_tree print_tree.cpp)
target_link_libraries(print_tree matrix)

