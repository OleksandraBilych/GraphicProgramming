cmake_minimum_required(VERSION 3.5)
project(glad)

add_library(glad glad.c)
target_include_directories(glad PUBLIC include/)

target_link_libraries(glad dl)
