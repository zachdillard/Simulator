cmake_minimum_required(VERSION 3.6)
project(Project)

set(CMAKE_CXX_STANDARD 11)

set(SOURCE_FILES Driver.cpp CPU.h loader.h Memory.h Schedulers.h ShortTerm.h)
add_executable(Project ${SOURCE_FILES})