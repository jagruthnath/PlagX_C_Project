cmake_minimum_required(VERSION 3.8)
project(PlagX_C_Project)

set(CMAKE_C_STANDARD 11)

set(SOURCE_FILES main.c Plagiarism.h)
add_executable(PlagX_C_Project ${SOURCE_FILES})