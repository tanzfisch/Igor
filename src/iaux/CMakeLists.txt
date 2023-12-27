cmake_minimum_required(VERSION 3.13)

message(STATUS "iaux")

project(iaux 
    DESCRIPTION "Auxilliary library for Igor Game Engine" 
    LANGUAGES CXX)

set(IAUX_SRC_DIR "${CMAKE_CURRENT_SOURCE_DIR}/iaux/")
set(IAUX_HEADERS_DIR "${CMAKE_CURRENT_SOURCE_DIR}")

file(GLOB_RECURSE IAUX_SOURCES LIST_DIRECTORIES false "${IAUX_SRC_DIR}*.c*") #TODO
file(GLOB_RECURSE IAUX_HEADERS LIST_DIRECTORIES false "${IAUX_SRC_DIR}*.h*" "${IAUX_SRC_DIR}*.inl") #TODO
list(APPEND IAUX_SOURCES ${IAUX_HEADERS})
add_library(iaux SHARED ${IAUX_SOURCES})

if(IS_GNU)
    target_compile_definitions(iaux PRIVATE -D__IAUX_BUILDING_DLL__ PUBLIC -D_cplusplus=201703L ${IGOR_GNU_BUILD_TYPE})
    target_compile_options(iaux PRIVATE -std=c++17)
    target_link_libraries(iaux PRIVATE stdc++fs Threads::Threads)
elseif(IS_MSVC)
    target_compile_definitions(iaux PRIVATE -D__IAUX_BUILDING_DLL__ PUBLIC -DNOMINMAX -D_UNICODE -DUNICODE)
    target_compile_options(iaux PRIVATE "/std:c++17" "/MP" "/W1")
    target_link_libraries(iaux PRIVATE DbgHelp Threads::Threads)
endif()

message(STATUS ${IAUX_HEADERS_DIR})

target_include_directories(iaux 
    PUBLIC
    ${IAUX_HEADERS_DIR})

source_group_special("${IAUX_SOURCES}" ${IAUX_SRC_DIR})  