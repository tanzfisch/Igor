cmake_minimum_required(VERSION 3.13)

message(STATUS "Glad")

project(Glad 
    VERSION 0.1.36 
    DESCRIPTION "Glad OpenGL loader" 
    LANGUAGES CXX)

add_library(Glad 
    STATIC
    src/glad.c
    include/glad/glad.h
    include/KHR/khrplatform.h)

if(IS_GNU)
    target_compile_options(Glad PRIVATE -std=c++17 -fPIC)
    target_compile_definitions(Glad PUBLIC -DGLX_GLXEXT_PROTOTYPES)
elseif(IS_MSVC)
    target_compile_options(Glad PRIVATE "/std:c++17" "/MP" "/W1")
endif()

target_include_directories(Glad PUBLIC include)