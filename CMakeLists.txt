cmake_minimum_required(VERSION 3.29)
project(untitled C)

set(CMAKE_C_STANDARD 11)

add_executable(untitled main.c
        include/grafo_lista.h
        src/grafo_lista.c
        src/grafo_matriz.c
        include/grafo_matriz.h
        src/grafo.c
        include/grafo.h)
