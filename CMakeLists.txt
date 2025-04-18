cmake_minimum_required(VERSION 3.28)
project(project)

set(CMAKE_CXX_STANDARD 17)

add_executable(project main.cpp)
target_compile_features(project PRIVATE cxx_std_23)
