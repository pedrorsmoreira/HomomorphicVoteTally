cmake_minimum_required(VERSION 3.10)

project(electionKey VERSION 1.0)

add_executable(electionKey electionKey.cpp)

find_package(SEAL 3.4 REQUIRED)
target_link_libraries(electionKey SEAL::seal)