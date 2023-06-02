cmake_minimum_required(VERSION 3.15)
project(PackageTest CXX)

find_package(backend CONFIG REQUIRED)
find_package(gtest CONFIG REQUIRED)

enable_testing()

add_executable(unit_tests src/unit_tests.cpp)
target_link_libraries (unit_tests backend::backend GTest::gtest_main)

include(GoogleTest)
gtest_discover_tests(unit_tests)