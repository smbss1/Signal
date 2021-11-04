cmake_minimum_required(VERSION 3.17)
project(Signal_Slot)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_BUILD_TYPE Release)

include(${CMAKE_BINARY_DIR}/conanbuildinfo.cmake)
conan_basic_setup()

file(GLOB_RECURSE SRCS "src/*.cpp")

add_library(fox_signal STATIC ${SRCS})
target_include_directories(fox_signal PUBLIC "include")

set(SIGNAL_EXAMPLE 1)

if (SIGNAL_EXAMPLE)
    add_subdirectory(tests)
endif()
