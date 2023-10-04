# @copyright Copyright (c) 2023. Created by Konstantin Belousov.
# All rights reserved.

cmake_minimum_required(VERSION 3.25)
project(random_sequences_generator)

set(CMAKE_CXX_STANDARD 20)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra -Wpedantic -Werror")

add_executable(random_sequences_generator main.cpp generator.h value-generators.h value-generators.cpp)
