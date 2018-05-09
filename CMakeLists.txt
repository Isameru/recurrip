
cmake_minimum_required(VERSION 3.5.1)

set_property(GLOBAL PROPERTY USE_FOLDERS ON)

project(recurrip CXX)

set(VERSION_MAJOR 0)
set(VERSION_MINOR 0)
set(VERSION_PATCH 1)

if(MSVC)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /std:c++latest")
else()
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++17")
endif()

set(CMAKE_CXX_STANDARD_REQUIRED on)

add_executable(recurrip
    src/recurrip.cpp
)
