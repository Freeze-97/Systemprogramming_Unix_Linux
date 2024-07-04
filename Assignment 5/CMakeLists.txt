cmake_minimum_required(VERSION 3.21)
project(DT065G_toya1800_labb5)

set(CMAKE_CXX_STANDARD 14)

add_executable(DT065G_toya1800_labb5 lab5.cpp
        Restart.h Restart.cc TCPsocket.h TCPsocket.cpp)
