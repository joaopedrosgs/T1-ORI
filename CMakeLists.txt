cmake_minimum_required(VERSION 3.9)
project(t1_ori)

set(CMAKE_C_STANDARD 99)

set(SOURCE_FILES main.c arquivo.c registro.c bloco.c )
add_executable(t1_ori ${SOURCE_FILES})
