cmake_minimum_required(VERSION 3.13)

message(STATUS "iaux")

project(iaux 
    DESCRIPTION "Auxilliary library for Igor Game Engine" 
    LANGUAGES CXX)

file(GLOB_RECURSE IAUX_SOURCES LIST_DIRECTORIES false "iaux/*.c*") #TODO
file(GLOB_RECURSE IAUX_HEADERS LIST_DIRECTORIES false "iaux/*.h*" "iaux/*.inl") #TODO
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

target_include_directories(iaux PUBLIC .)
