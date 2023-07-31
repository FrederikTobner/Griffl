cmake_minimum_required(VERSION 3.18.0)
project(Griffl VERSION 0.1.0 LANGUAGES C)
set(BUILD_SHARED_LIBS OFF CACHE BOOL "" FORCE)
add_subdirectory(src)
# Forcing static linking of the libraries - dirty fix for building all SDL libraries as static libraries
add_subdirectory(external)
set(CMAKE_C_STANDARD 99)
set(C_STANDARD_REQUIRED TRUE)
FILE(COPY fonts/ DESTINATION "${CMAKE_BINARY_DIR}/fonts")