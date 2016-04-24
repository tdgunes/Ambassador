cmake_minimum_required(VERSION 3.2)
project(Ambassador)

list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_LIST_DIR}/external")
find_package(Libevent2 REQUIRED)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

include_directories(${LIBEVENT2_INCLUDE_DIR} include/ libs/)

set(SOURCE_FILES
        ambassador.cpp
        src/Client.cpp
        include/Client.h
        src/Server.cpp
        include/Server.h
        libs/json.hpp
        src/Handler.cpp
        include/Handler.h)

add_executable(ambassador ${SOURCE_FILES})
target_link_libraries(ambassador ${LIBEVENT2_LIBRARIES})
