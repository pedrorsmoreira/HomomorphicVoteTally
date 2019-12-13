cmake_minimum_required(VERSION 3.10)

project(tally VERSION 1.0)

add_executable(tally tally.cpp)

find_package(SEAL 3.4 REQUIRED)
target_link_libraries(tally SEAL::seal)