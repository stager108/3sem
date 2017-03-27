cmake_minimum_required(VERSION 3.6)
project(dz3)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

set(SOURCE_FILES
    problemA.cpp
    problemF.cpp)


#add_executable(TaskF problemF.cpp)
add_executable(TaskA problemA.cpp)
add_executable(TaskH problemH.cpp)
