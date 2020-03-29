cmake_minimum_required(VERSION 3.16)
project(HW_Archiver)

set(CMAKE_CXX_STANDARD 17)

add_executable(HW_Archiver src/main.cpp lib/bitbuf.hpp lib/timer.hpp lib/types.h lib/utils.h lib/archiver.hpp lib/huffman.hpp lib/lz77.hpp lib/lzw.hpp)