cmake_minimum_required(VERSION 3.10)
project(QuantProject)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Optimzation to run the code in Release mode instead of Debug mode
# For Mac and Linux systems
if(NOT MSVC)
    add_compile_options(-O3 -march=native)
    add_definitions(-DNDEBUG)
# For Windows system
else()
    add_compile_options(/O2 /arch:AVX2) 
    add_definitions(-DNDEBUG) 
    add_definitions(-D_CRT_SECURE_NO_WARNINGS)
endif()

find_package(xtensor REQUIRED)

add_executable(main main.cpp)

target_link_libraries(main PRIVATE xtensor)