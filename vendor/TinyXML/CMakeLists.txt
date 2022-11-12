cmake_minimum_required(VERSION 3.13)

message(STATUS "TinyXML")

project(TinyXML 
    VERSION 0.1.36 
    DESCRIPTION "TinyXML is a simple, small, C++ XML parser that can be easily integrated into other programs." 
    LANGUAGES CXX)

add_library(TinyXML 
    STATIC
    tinystr.cpp
    tinystr.h
    tinyxml.cpp
    tinyxml.h
    tinyxmlerror.cpp
    tinyxmlparser.cpp)

if(IS_GNU)
    target_compile_options(TinyXML PRIVATE -std=c++17 -fPIC)
elseif(IS_MSVC)
    target_compile_options(TinyXML PRIVATE "/std:c++17" "/MP" "/W2")
endif()

target_include_directories(TinyXML PUBLIC .)