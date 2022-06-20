cmake_minimum_required(VERSION 3.22)

#Initiating Project
project(cmake-build-tool)


#Setting C++ Standard
set(CMAKE_CXX_STANDARD 20)

#Adding the files
add_executable(${PROJECT_NAME} "./src/main.cpp" "./ANSI-Library/ansi_lib.h")