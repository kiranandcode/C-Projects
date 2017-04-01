cmake_minimum_required(VERSION 3.6)
project(MatrixProgram)

set(CMAKE_C_STANDARD 11)

set(SOURCE_FILES main.c matrix.c matrix.h)
add_executable(MatrixProgram ${SOURCE_FILES})