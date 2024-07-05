cmake_minimum_required(VERSION 3.15...3.25)

if (NOT DEFINED CMAKE_CXX_STANDARD)
    set(CMAKE_CXX_STANDARD 20)
endif()
set(CMAKE_CXX_EXTENSIONS OFF)

project(jsonsimplecpp
        VERSION 0.1
        LANGUAGES CXX)

include(ProjectOptions.cmake)

set_local_options()

target_compile_features(jsonsimplecpp_options INTERFACE cxx_std_${CMAKE_CXX_STANDARD})

add_subdirectory(src)