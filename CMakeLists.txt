# Minimum CMake version required
cmake_minimum_required(VERSION 3.13.0)

# Project (also works w/o version)
project(My_Project VERSION 1.0.0)

# C++ standard
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED True)


# ---------------------
# Libraries
# ----------------------

# Dynamic library
find_library(zmq_LIBRARY libzmq${CMAKE_SHARED_LIBRARY_SUFFIX}
    HINTS /Users/andreas/Documents/programming/zeroMQ/zmqLibrary/lib
    REQUIRED)

message("-- zmq_LIBRARY = ${zmq_LIBRARY}")

# Static library
find_library(zmq_LIBRARY_STATIC libzmq${CMAKE_STATIC_LIBRARY_SUFFIX}
    HINTS /Users/andreas/Documents/programming/zeroMQ/zmqLibrary/lib
    REQUIRED)

message("-- zmq_LIBRARY_STATIC = ${zmq_LIBRARY_STATIC}")

# cppzmq
find_path(cppzmq_INCLUDE_DIR zmq.hpp
         HINTS /Users/andreas/Documents/programming/zeroMQ/zmqLibrary/include
         REQUIRED)

message("-- cppzmq_INCLUDE_DIR = ${cppzmq_INCLUDE_DIR}")


# ---------------------
# Targets
# ----------------------
# Client
add_executable(client src/client.cpp)
target_include_directories(client PRIVATE ${cppzmq_INCLUDE_DIR} )
target_link_libraries(client PRIVATE ${zmq_LIBRARY})

# Server
add_executable(server src/server.cpp)
target_include_directories(server PRIVATE ${cppzmq_INCLUDE_DIR} )
target_link_libraries(server PRIVATE ${zmq_LIBRARY})



