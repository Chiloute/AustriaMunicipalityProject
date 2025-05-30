cmake_minimum_required(VERSION 3.30)
project(SemesterProject)

set(CMAKE_CXX_STANDARD 20)

add_executable(SemesterProject src/main.cpp
        src/Territorial.h
        src/DataLoader.cpp
        src/DataLoader.h
        src/Predicates.h
        src/filters.cpp
        src/filters.h
        src/heap_monitor.h
	src/sort.cpp
	src/sort.h
)
