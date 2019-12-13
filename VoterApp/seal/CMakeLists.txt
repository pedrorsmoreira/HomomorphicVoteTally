cmake_minimum_required(VERSION 3.10)

project(seal_encrypt VERSION 1.0)

add_executable(seal_encrypt seal_encrypt.cpp)

find_package(SEAL 3.4 REQUIRED)
target_link_libraries(seal_encrypt SEAL::seal)
