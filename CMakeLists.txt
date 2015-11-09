cmake_minimum_required(VERSION 3.3)
project(Ambassador)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

include_directories( ${OpenCV_INCLUDE_DIRS}
        include/)

set(SOURCE_FILES ambassador.cpp src/Server.cpp include/Server.h)
add_executable(ambassador ${SOURCE_FILES})