cmake_minimum_required(VERSION 3.6)
project(Simulator)

set(CMAKE_CXX_STANDARD 11)

set(SOURCE_FILES Driver.cpp Loader.h Dispatcher.h Scheduler.h CPU.h Structs.h)
add_executable(Simulator ${SOURCE_FILES})