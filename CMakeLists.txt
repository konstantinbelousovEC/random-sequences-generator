cmake_minimum_required(VERSION 3.25)
project(random_sequences_generator)

set(CMAKE_CXX_STANDARD 17)

add_executable(random_sequences_generator main.cpp sfinae-defs.h generator.h)
