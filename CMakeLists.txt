cmake_minimum_required(VERSION 3.15)
project(backend CXX)

add_library(backend src/Book.cpp)
target_include_directories(backend PUBLIC include)

set_target_properties(backend PROPERTIES PUBLIC_HEADER "include/Book.h;include/BookInterface.h")
install(TARGETS backend)
