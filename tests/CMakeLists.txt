cmake_minimum_required(VERSION 3.0)
project(xcard_tests VERSION 1.0 LANGUAGES CXX)
set(CMAKE_CXX_STANDARD 11)

include_directories(../include /usr/include)

add_executable(xcard_tests xcard_tests.cpp ../src/xcard.cpp ../src/xcard_mt.cpp)

target_link_libraries(xcard_tests PRIVATE pthread)
target_link_libraries(xcard_tests PRIVATE gtest)
