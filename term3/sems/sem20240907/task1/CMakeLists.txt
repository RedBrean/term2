cmake_minimum_required(VERSION 3.20)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)
set(CMAKE_CXX_COMPILER "g++")

project(Some_Project LANGUAGES CXX)

add_executable(main)

target_sources(main PRIVATE main.cpp)

