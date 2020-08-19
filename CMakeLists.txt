cmake_minimum_required(VERSION 3.0)
project(xcard VERSION 1.0 LANGUAGES CXX)
set(CMAKE_CXX_STANDARD 11)

include_directories(include /usr/include)

add_executable(demo src/demo.cpp src/xcard.cpp src/xcard_mt.cpp)

add_library(libxcard MODULE src/xcard.cpp src/xcard_mt.cpp)
set_target_properties(libxcard
    PROPERTIES
    PREFIX ""
)

target_link_libraries(demo PRIVATE pthread)