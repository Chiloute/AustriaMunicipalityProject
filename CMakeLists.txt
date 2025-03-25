cmake_minimum_required(VERSION 3.30)
project(SemesterProject)

set(CMAKE_CXX_STANDARD 20)

add_executable(SemesterProject src/main.cpp
        src/Municipality.h
        src/DataLoader.cpp
        src/DataLoader.h
        "src/MunicipalityProcessor.cpp"
        "src/MunicipalityProcessor.h"
        src/Predicates.h
)
