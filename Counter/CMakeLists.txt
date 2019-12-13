cmake_minimum_required(VERSION 3.10)

project(counter VERSION 1.0)

add_executable(counter counter.cpp)

find_package(SEAL 3.4 REQUIRED)
target_link_libraries(counter SEAL::seal)