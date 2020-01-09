cmake_minimum_required(VERSION 3.15)
project(rcwt)

set(CMAKE_CXX_STANDARD 14)

add_executable(rcwt src/rcwt.h src/main.cpp src/Converter.h src/Converter.cpp src/VirtualMachine.h src/VirtualMachine.cpp src/JITCompiler.cpp src/JITCompiler.h)