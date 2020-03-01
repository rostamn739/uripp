cmake_minimum_required(VERSION 2.8)
project(uripp)

set(CMAKE_CXX_STANDARD 17)

add_subdirectory(src)
add_subdirectory(test)
add_subdirectory(vendor/googletest)
