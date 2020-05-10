cmake_minimum_required(VERSION 3.0)

project(Igor)

# global definitions
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
add_definitions(-D_UNICODE -DNOMINMAX -D_NEWTON_USE_DOUBLE)

# 3rd party definitions
#tinyXML
set(TINYXML_HEADERS_DIR "${CMAKE_SOURCE_DIR}/3rd/TinyXML/")
file(GLOB TINYXML_SOURCES RELATIVE ${CMAKE_SOURCE_DIR} "3rd/TinyXML/*.cpp")
file(GLOB TINYXML_HEADERS RELATIVE ${CMAKE_SOURCE_DIR} "3rd/TinyXML/*.h")

# GLee
set(GLEE_HEADERS_DIR "${CMAKE_SOURCE_DIR}/3rd/GLee/include/")
file(GLOB GLEE_SOURCES RELATIVE ${CMAKE_SOURCE_DIR} "3rd/GLee/src/*.c")
file(GLOB GLEE_HEADERS RELATIVE ${CMAKE_SOURCE_DIR} "3rd/GLee/include/*.h")

# newton dynamics
set(TIME_TRACKER_HEADERS_DIR "${CMAKE_SOURCE_DIR}/3rd/newton-dynamics/packages/thirdParty/timeTracker/")

set(NEWTON_CORE_HEADERS_DIR "${CMAKE_SOURCE_DIR}/3rd/newton-dynamics/coreLibrary_300/source/core/")
file(GLOB NEWTON_CORE_SOURCES RELATIVE ${CMAKE_SOURCE_DIR} "3rd/newton-dynamics/coreLibrary_300/source/core/*.cpp")
file(GLOB NEWTON_CORE_HEADERS RELATIVE ${CMAKE_SOURCE_DIR} "3rd/newton-dynamics/coreLibrary_300/source/core/*.h")

set(NEWTON_MESH_UTIL_HEADERS_DIR "${CMAKE_SOURCE_DIR}/3rd/newton-dynamics/coreLibrary_300/source/meshUtil/")

set(NEWTON_PHYSICS_HEADERS_DIR "${CMAKE_SOURCE_DIR}/3rd/newton-dynamics/coreLibrary_300/source/physics/")
file(GLOB NEWTON_PHYSICS_SOURCES RELATIVE ${CMAKE_SOURCE_DIR} "3rd/newton-dynamics/coreLibrary_300/source/physics/*.cpp")
file(GLOB NEWTON_PHYSICS_HEADERS RELATIVE ${CMAKE_SOURCE_DIR} "3rd/newton-dynamics/coreLibrary_300/source/physics/*.h")

set(NEWTON_HEADERS_DIR "${CMAKE_SOURCE_DIR}/3rd/newton-dynamics/coreLibrary_300/source/")
file(GLOB NEWTON_SOURCES RELATIVE ${CMAKE_SOURCE_DIR} "3rd/newton-dynamics/coreLibrary_300/source/newton/*.cpp")
file(GLOB NEWTON_HEADERS RELATIVE ${CMAKE_SOURCE_DIR} "3rd/newton-dynamics/coreLibrary_300/source/newton/*.h")

# igor definitions
# IgorAux
set(IGOR_AUX_HEADERS_DIR "${CMAKE_SOURCE_DIR}/src/IgorAux/src/")
file(GLOB_RECURSE IGOR_AUX_SOURCES RELATIVE ${CMAKE_SOURCE_DIR} "src/IgorAux/src/*.cpp")
file(GLOB_RECURSE IGOR_AUX_HEADERS RELATIVE ${CMAKE_SOURCE_DIR} "src/IgorAux/src/*.h")

# Igor
set(STB_IMAGE_HEADERS_DIR "${CMAKE_SOURCE_DIR}/3rd/stb_image/")
set(IGOR_HEADERS_DIR "${CMAKE_SOURCE_DIR}/src/Igor/src/")
file(GLOB_RECURSE IGOR_SOURCES RELATIVE ${CMAKE_SOURCE_DIR} "src/Igor/src/*.cpp")
file(GLOB_RECURSE IGOR_HEADERS RELATIVE ${CMAKE_SOURCE_DIR} "src/Igor/src/*.h")

# OMPF
set(OMPF_HEADERS_DIR "${CMAKE_SOURCE_DIR}/src/OMPF/src/")
file(GLOB_RECURSE OMPF_SOURCES RELATIVE ${CMAKE_SOURCE_DIR} "src/OMPF/src/*.cpp")
file(GLOB_RECURSE OMPF_HEADERS RELATIVE ${CMAKE_SOURCE_DIR} "src/OMPF/src/*.h")

# MICA
set(MICA_HEADERS_DIR "${CMAKE_SOURCE_DIR}/src/Tools/Mica/src/")
file(GLOB_RECURSE MICA_SOURCES RELATIVE ${CMAKE_SOURCE_DIR} "src/Tools/Mica/src/*.cpp")
file(GLOB_RECURSE MICA_HEADERS RELATIVE ${CMAKE_SOURCE_DIR} "src/Tools/Mica/src/*.h")

# setup newton build
add_library(core STATIC ${NEWTON_CORE_SOURCES} ${NEWTON_CORE_HEADERS})
if (MSVC_VERSION GREATER_EQUAL "1900")
    target_compile_options(core PRIVATE "/MP")
endif()
target_compile_definitions(core PRIVATE -D_NEWTON_USE_DOUBLE)
target_include_directories(core PRIVATE  ${NEWTON_CORE_HEADERS_DIR} ${TIME_TRACKER_HEADERS_DIR})

add_library(physics STATIC ${NEWTON_PHYSICS_SOURCES} ${NEWTON_PHYSICS_HEADERS})
if (MSVC_VERSION GREATER_EQUAL "1900")
  target_compile_options(physics PRIVATE "/MP")
endif()
target_compile_definitions(physics PRIVATE -D_NEWTON_USE_DOUBLE)
target_include_directories(physics PRIVATE  ${NEWTON_PHYSICS_HEADERS_DIR} ${NEWTON_CORE_HEADERS_DIR} ${TIME_TRACKER_HEADERS_DIR} ${NEWTON_MESH_UTIL_HEADERS_DIR})

add_library(newton STATIC ${NEWTON_SOURCES} ${NEWTON_HEADERS})
if (MSVC_VERSION GREATER_EQUAL "1900")
  target_compile_options(newton PRIVATE "/MP")
endif()
target_compile_definitions(newton PRIVATE -D_NEWTON_USE_DOUBLE -D_NEWTON_STATIC_LIB)
target_include_directories(newton PRIVATE  ${NEWTON_HEADERS_DIR} ${NEWTON_PHYSICS_HEADERS_DIR} ${NEWTON_CORE_HEADERS_DIR} ${TIME_TRACKER_HEADERS_DIR} ${NEWTON_MESH_UTIL_HEADERS_DIR})

# setup IgorAux build
add_library(IgorAux STATIC ${IGOR_AUX_SOURCES} ${IGOR_AUX_HEADERS} ${TINYXML_AUX_SOURCES} ${TINYXML_AUX_HEADERS})
if (MSVC_VERSION GREATER_EQUAL "1900")
    target_compile_options(IgorAux PRIVATE "/std:c++latest" "/MP")
endif()
target_compile_definitions(IgorAux PRIVATE -D__IGOR_AUX_BUILDING_DLL__)
target_include_directories(IgorAux PRIVATE  ${IGOR_AUX_HEADERS_DIR} ${TINYXML_HEADERS_DIR})

# setup OMPF build
add_library(OMPF STATIC ${OMPF_SOURCES} ${OMPF_HEADERS})
if (MSVC_VERSION GREATER_EQUAL "1900")
    target_compile_options(OMPF PRIVATE "/std:c++latest" "/MP")
endif()
target_compile_definitions(OMPF PRIVATE -D__OMPF_BUILDING_DLL__)
target_include_directories(OMPF PRIVATE  ${OMPF_HEADERS_DIR} ${IGOR_AUX_HEADERS_DIR})
target_link_libraries(OMPF IgorAux)

# setup Igor build
add_library(Igor SHARED ${IGOR_SOURCES} ${IGOR_HEADERS} ${GLEE_SOURCES} ${GLEE_HEADERS})
if (MSVC_VERSION GREATER_EQUAL "1900")
    target_compile_options(Igor PRIVATE "/std:c++latest" "/MP")
endif()
target_compile_definitions(Igor PRIVATE -D__IGOR_BUILDING_DLL__)
target_include_directories(Igor PRIVATE  ${IGOR_HEADERS_DIR} ${IGOR_AUX_HEADERS_DIR} ${OMPF_HEADERS_DIR} ${GLEE_HEADERS_DIR} ${TINYXML_HEADERS_DIR} ${STB_IMAGE_HEADERS_DIR} ${NEWTON_PHYSICS_HEADERS_DIR} ${NEWTON_CORE_HEADERS_DIR} ${NEWTON_HEADERS_DIR})
target_link_libraries(Igor OMPF IgorAux core physics newton)


#add_library(OMPF ${OMPF_SOURCES})
# add_library(Igor foo.cpp)
# add_executable(app main.cpp)
#target_link_libraries(IgorAux)


#install(TARGETS IgorAux DESTINATION bin)