cmake_minimum_required(VERSION 3.13)
project(statey)

set(CMAKE_CXX_STANDARD 20)

add_subdirectory(3rdparty/google_test)

include_directories(include)
include_directories(3rdparty/boost_type_traits/include)
include_directories(3rdparty/boost_config/include)
include_directories(3rdparty/boost_static_assert/include)
include_directories(3rdparty/google_test/googletest/include)

add_executable(Google_Tests_run test/state_machine_test.cpp)
target_link_libraries(Google_Tests_run gtest gtest_main)