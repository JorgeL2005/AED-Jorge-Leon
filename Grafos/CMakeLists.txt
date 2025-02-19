cmake_minimum_required(VERSION 3.26)
project(Grafos)

set(CMAKE_CXX_STANDARD 17)

add_executable(Grafos grafov1.cpp
        grafov2.cpp
        bellmanford.cpp)
