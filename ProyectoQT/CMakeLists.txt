cmake_minimum_required(VERSION 3.26)
project(ProyectoQT)

set(CMAKE_CXX_STANDARD 17)

# Agrega la carpeta include
include_directories(${CMAKE_SOURCE_DIR})

add_executable(ProyectoQT main.cpp stb_image.h stb_image_write.h
        stb_image_resize.h)
