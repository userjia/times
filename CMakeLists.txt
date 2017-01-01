cmake_minimum_required(VERSION 3.6)
project(times)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")
link_libraries("pthread")

#set(SOURCE_FILES  main.h main.c config.c socket.c msgq.c sync.c)
set(SOURCE_FILES main.c sync.c main.h socket.c msgq.c config.c )
add_executable(times ${SOURCE_FILES})