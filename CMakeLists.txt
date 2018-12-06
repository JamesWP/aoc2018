cmake_minimum_required(VERSION 3.12)
project(aoc2018)

find_package(GTest REQUIRED)

file(GLOB DAYS day*.cpp)

add_executable(aoc ${DAYS})
target_include_directories(aoc PRIVATE ${GTEST_INCLUDE_DIRS})
target_link_libraries(aoc PRIVATE ${GTEST_BOTH_LIBRARIES} pthread)

