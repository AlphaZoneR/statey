cmake_minimum_required(VERSION 3.13)
project(statey)

set(CMAKE_CXX_STANDARD 20)

include_directories(include)
include_directories(3rdparty/boost_type_traits/include)
include_directories(3rdparty/boost_config/include)
include_directories(3rdparty/boost_static_assert/include)

add_library(statey include/state_machine.hpp)
set_target_properties(statey PROPERTIES LINKER_LANGUAGE CXX)