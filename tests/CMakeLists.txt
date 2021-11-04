cmake_minimum_required(VERSION 3.17)

enable_testing()

file(GLOB_RECURSE TESTS_SRCS "*.cpp")

add_executable(signal_tests ${TESTS_SRCS})
target_link_libraries(signal_tests ${CONAN_LIBS} fox_signal)

add_test(signal_tests signal_tests)
