cmake_minimum_required(VERSION 2.8)
project(uripp)

set(CMAKE_CXX_STANDARD 17)
set(GTEST_ROOT "${CMAKE_SOURCE_DIR}/vendor/googletest/")

add_subdirectory(src)
add_subdirectory(test)
add_subdirectory(vendor/googletest)
