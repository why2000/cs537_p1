cmake_minimum_required(VERSION 3.17)
project(cs537_p1 C)

set(CMAKE_C_STANDARD 99)

add_executable(cs537_p1 537ps.c procParser.c procParser.h optionParser.c optionParser.h dirParser.c dirParser.h)