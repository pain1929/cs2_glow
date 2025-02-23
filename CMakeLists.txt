cmake_minimum_required(VERSION 3.5)


project(glow VERSION 1.0.0)


add_library(${PROJECT_NAME} SHARED dllMain.cpp)