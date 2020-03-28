cmake_minimum_required(VERSION 3.16)
project(HW_Archiver)

set(CMAKE_CXX_STANDARD 17)

add_executable(HW_Archiver src/main.cpp lib/bitbuf.h lib/timer.h lib/types.h lib/utils.h lib/archiver.h lib/huffman.h lib/huffman.cpp lib/lz77.h lib/lzw.h)