cmake_minimum_required(VERSION 3.10)

project(counter_test VERSION 1.0)

add_library(randombytes sss/randombytes.c)
add_library(hazmat sss/hazmat.c)
add_library(tweet sss/tweetnacl.c)
add_library(sss sss/sss.c)

add_executable(counter_test counter_test.cpp)

target_link_libraries(sss randombytes)
target_link_libraries(sss hazmat)
target_link_libraries(sss tweet)
target_link_libraries(counter_test sss)

find_package(SEAL 3.4 REQUIRED)
target_link_libraries(counter_test SEAL::seal)