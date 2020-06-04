cmake_minimum_required(VERSION 3.13)

project(heapsort)

add_executable(heapsort heapsort.c binaryheap.c binaryheap.h)

target_link_libraries(heapsort m)
