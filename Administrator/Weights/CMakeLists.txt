cmake_minimum_required(VERSION 3.10)

project(weights VERSION 1.0)

add_executable(weights weights.cpp)

find_package(SEAL 3.4 REQUIRED)
target_link_libraries(weights SEAL::seal)