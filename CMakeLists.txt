cmake_minimum_required(VERSION 3.0)

project(Igor)

if (CMAKE_CXX_COMPILER_ID STREQUAL "GNU")

    MESSAGE("using GCC")
    add_compile_options("-fpermissive" "--std=c++17")
    add_definitions(-D_NEWTON_USE_DOUBLE)

elseif (CMAKE_CXX_COMPILER_ID STREQUAL "MSVC")

    if (MSVC_VERSION GREATER_EQUAL "1900")
        MESSAGE("using Visual Studio C++")
        add_compile_options("/std:c++latest" "/MP")
        add_definitions(-D_UNICODE -DUNICODE -DNOMINMAX -D_NEWTON_USE_DOUBLE)
    elseif()
        MESSAGE(FATAL_ERROR "unsupported compiler version")
    endif()

endif()

# global definitions
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)


# 3rd party definitions
#tinyXML
set(TINYXML_HEADERS_DIR "${CMAKE_SOURCE_DIR}/3rd/TinyXML/")
set(TINYXML_SOURCES 
    "${TINYXML_HEADERS_DIR}/tinystr.cpp"
    "${TINYXML_HEADERS_DIR}/tinyxml.cpp"
    "${TINYXML_HEADERS_DIR}/tinyxmlerror.cpp"
    "${TINYXML_HEADERS_DIR}/tinyxmlparser.cpp")
set(TINYXML_HEADERS 
    "${TINYXML_HEADERS_DIR}/tinystr.h"
    "${TINYXML_HEADERS_DIR}/tinyxml.h")

# GLee
set(GLEE_HEADERS_DIR "${CMAKE_SOURCE_DIR}/3rd/GLee/include/")
file(GLOB GLEE_SOURCES RELATIVE ${CMAKE_SOURCE_DIR} "3rd/GLee/src/*.c")
file(GLOB GLEE_HEADERS RELATIVE ${CMAKE_SOURCE_DIR} "3rd/GLee/include/*.h")

# stb_image
set(STB_IMAGE_HEADERS_DIR "${CMAKE_SOURCE_DIR}/3rd/stb_image/")

# newton dynamics
set(TIME_TRACKER_HEADERS_DIR "${CMAKE_SOURCE_DIR}/3rd/newton-dynamics/packages/thirdParty/timeTracker/")

set(NEWTON_CORE_HEADERS_DIR "${CMAKE_SOURCE_DIR}/3rd/newton-dynamics/coreLibrary_300/source/core/")
file(GLOB NEWTON_CORE_SOURCES RELATIVE ${CMAKE_SOURCE_DIR} "3rd/newton-dynamics/coreLibrary_300/source/core/*.c*")
file(GLOB NEWTON_CORE_HEADERS RELATIVE ${CMAKE_SOURCE_DIR} "3rd/newton-dynamics/coreLibrary_300/source/core/*.h*")

set(NEWTON_MESH_UTIL_HEADERS_DIR "${CMAKE_SOURCE_DIR}/3rd/newton-dynamics/coreLibrary_300/source/meshUtil/")
file(GLOB NEWTON_MESH_UTIL_SOURCES RELATIVE ${CMAKE_SOURCE_DIR} "3rd/newton-dynamics/coreLibrary_300/source/meshUtil/*.c*")
file(GLOB NEWTON_MESH_UTIL_HEADERS RELATIVE ${CMAKE_SOURCE_DIR} "3rd/newton-dynamics/coreLibrary_300/source/meshUtil/*.h*")

set(NEWTON_PHYSICS_HEADERS_DIR "${CMAKE_SOURCE_DIR}/3rd/newton-dynamics/coreLibrary_300/source/physics/")
file(GLOB NEWTON_PHYSICS_SOURCES RELATIVE ${CMAKE_SOURCE_DIR} "3rd/newton-dynamics/coreLibrary_300/source/physics/*.c*")
file(GLOB NEWTON_PHYSICS_HEADERS RELATIVE ${CMAKE_SOURCE_DIR} "3rd/newton-dynamics/coreLibrary_300/source/physics/*.h*")

set(NEWTON_HEADERS_DIR "${CMAKE_SOURCE_DIR}/3rd/newton-dynamics/coreLibrary_300/source/")
file(GLOB NEWTON_SOURCES RELATIVE ${CMAKE_SOURCE_DIR} "3rd/newton-dynamics/coreLibrary_300/source/newton/*.c*")
file(GLOB NEWTON_HEADERS RELATIVE ${CMAKE_SOURCE_DIR} "3rd/newton-dynamics/coreLibrary_300/source/newton/*.h*")

# setup newton build
message(Newton)
add_library(core STATIC ${NEWTON_CORE_SOURCES} ${NEWTON_CORE_HEADERS})
if (CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
    target_compile_definitions(core PRIVATE -D_POSIX_VER_64)
    target_compile_options(core PRIVATE -fpic -msse -msse3 -msse4 -mfpmath=sse -ffloat-store -ffast-math -freciprocal-math -funsafe-math-optimizations -fsingle-precision-constant)
endif()
target_include_directories(core PRIVATE ${NEWTON_CORE_HEADERS_DIR} ${TIME_TRACKER_HEADERS_DIR})

add_library(physics STATIC ${NEWTON_PHYSICS_SOURCES} ${NEWTON_PHYSICS_HEADERS})
if (CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
    target_compile_definitions(physics PRIVATE -D_POSIX_VER_64)
    target_compile_options(physics PRIVATE -fpic -msse -msse3 -msse4 -mfpmath=sse -ffloat-store -ffast-math -freciprocal-math -funsafe-math-optimizations -fsingle-precision-constant)
endif()
target_include_directories(physics PRIVATE  ${NEWTON_PHYSICS_HEADERS_DIR} ${NEWTON_CORE_HEADERS_DIR} ${TIME_TRACKER_HEADERS_DIR} ${NEWTON_MESH_UTIL_HEADERS_DIR})

add_library(meshUtil STATIC ${NEWTON_MESH_UTIL_SOURCES} ${NEWTON_MESH_UTIL_HEADERS})
if (CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
target_compile_definitions(meshUtil PRIVATE -D_POSIX_VER_64)
    target_compile_options(meshUtil PRIVATE -fpic -msse -msse3 -msse4 -mfpmath=sse -ffloat-store -ffast-math -freciprocal-math -funsafe-math-optimizations -fsingle-precision-constant)
endif()
target_include_directories(meshUtil PRIVATE  ${NEWTON_MESH_UTIL_HEADERS_DIR} ${NEWTON_CORE_HEADERS_DIR} ${TIME_TRACKER_HEADERS_DIR}  ${NEWTON_PHYSICS_HEADERS_DIR})

add_library(Newton SHARED ${NEWTON_SOURCES} ${NEWTON_HEADERS})
if (CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
    target_compile_definitions(Newton PRIVATE -D_POSIX_VER_64)
    target_compile_options(Newton PRIVATE -fpic -msse -msse3 -msse4 -mfpmath=sse -ffloat-store -ffast-math -freciprocal-math -funsafe-math-optimizations -fsingle-precision-constant)
endif()
target_compile_definitions(Newton PRIVATE -D_NEWTON_BUILD_DLL)
target_include_directories(Newton PRIVATE ${NEWTON_HEADERS_DIR} ${NEWTON_MESH_UTIL_HEADERS_DIR} ${NEWTON_CORE_HEADERS_DIR} ${TIME_TRACKER_HEADERS_DIR}  ${NEWTON_PHYSICS_HEADERS_DIR})
target_link_libraries(Newton core physics meshUtil)

# IgorAux
message(IgorAux)
set(IGOR_AUX_SRC_DIR "${CMAKE_SOURCE_DIR}/src/IgorAux/src/")
set(IGOR_AUX_HEADERS_DIR ${IGOR_AUX_SRC_DIR})
file(GLOB_RECURSE IGOR_AUX_SOURCES LIST_DIRECTORIES false "${IGOR_AUX_SRC_DIR}*.c*")
file(GLOB_RECURSE IGOR_AUX_HEADERS LIST_DIRECTORIES false "${IGOR_AUX_SRC_DIR}*.h*")
list(APPEND IGOR_AUX_SOURCES ${IGOR_AUX_HEADERS})

add_library(IgorAux SHARED ${IGOR_AUX_SOURCES})

foreach(_source IN ITEMS ${IGOR_AUX_SOURCES})
    get_filename_component(_source_path "${_source}" PATH)
    file(RELATIVE_PATH _source_path_rel "${IGOR_AUX_SRC_DIR}" "${_source_path}")
    string(REPLACE "/" "\\" _group_path "${_source_path_rel}")
    source_group("${_group_path}" FILES "${_source}")
endforeach()

target_compile_definitions(IgorAux PRIVATE -D__IGOR_AUX_BUILDING_DLL__)
target_include_directories(IgorAux PRIVATE  ${IGOR_AUX_HEADERS_DIR})

if (CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
    target_link_libraries(IgorAux)
elseif (CMAKE_CXX_COMPILER_ID STREQUAL "MSVC")
    target_link_libraries(IgorAux DbgHelp)
endif()

# OMPF
message("OMPF")

set(OMPF_SRC_DIR "${CMAKE_SOURCE_DIR}/src/OMPF/src/")
set(OMPF_HEADERS_DIR ${OMPF_SRC_DIR})

file(GLOB_RECURSE OMPF_SOURCES LIST_DIRECTORIES false "${OMPF_SRC_DIR}*.c*")
file(GLOB_RECURSE OMPF_HEADERS LIST_DIRECTORIES false "${OMPF_HEADERS_DIR}*.h*")
list(APPEND OMPF_SOURCES ${OMPF_HEADERS})

add_library(OMPF SHARED ${OMPF_SOURCES})

foreach(_source IN ITEMS ${OMPF_SOURCES})
    get_filename_component(_source_path "${_source}" PATH)
    file(RELATIVE_PATH _source_path_rel "${OMPF_SRC_DIR}" "${_source_path}")
    string(REPLACE "/" "\\" _group_path "${_source_path_rel}")
    source_group("${_group_path}" FILES "${_source}")
endforeach()

target_compile_definitions(OMPF PRIVATE -D__OMPF_BUILDING_DLL__)
target_include_directories(OMPF PRIVATE  ${OMPF_HEADERS_DIR} ${IGOR_AUX_HEADERS_DIR})
target_link_libraries(OMPF IgorAux)

# Igor
message("Igor")

set(IGOR_SRC_DIR "${CMAKE_SOURCE_DIR}/src/Igor/src/")
set(IGOR_HEADERS_DIR ${IGOR_SRC_DIR})

file(GLOB_RECURSE IGOR_SOURCES LIST_DIRECTORIES false "${IGOR_SRC_DIR}*.c*")
file(GLOB_RECURSE IGOR_HEADERS LIST_DIRECTORIES false "${IGOR_HEADERS_DIR}*.h*")
list(APPEND IGOR_SOURCES ${IGOR_HEADERS})

add_library(Igor SHARED ${IGOR_SOURCES} ${IGOR_HEADERS} ${GLEE_SOURCES} ${GLEE_HEADERS} ${TINYXML_SOURCES} ${TINYXML_HEADERS})

target_compile_definitions(Igor PRIVATE -D__IGOR_BUILDING_DLL__)
target_include_directories(Igor PRIVATE  ${IGOR_HEADERS_DIR} ${IGOR_AUX_HEADERS_DIR} ${OMPF_HEADERS_DIR} ${GLEE_HEADERS_DIR} ${TINYXML_HEADERS_DIR} ${STB_IMAGE_HEADERS_DIR} ${NEWTON_PHYSICS_HEADERS_DIR} ${NEWTON_CORE_HEADERS_DIR} ${NEWTON_HEADERS_DIR})
target_link_libraries(Igor IgorAux OMPF OpenGL32 glu32 Newton)

# MICA
set(MICA_HEADERS_DIR "${CMAKE_SOURCE_DIR}/src/Tools/Mica/src/")
file(GLOB_RECURSE MICA_SOURCES RELATIVE ${CMAKE_SOURCE_DIR} "src/Tools/Mica/src/*.c*")
file(GLOB_RECURSE MICA_HEADERS RELATIVE ${CMAKE_SOURCE_DIR} "src/Tools/Mica/src/*.h*")
add_executable(Mica ${MICA_SOURCES} ${MICA_HEADERS} )
target_include_directories(Mica PRIVATE  ${IGOR_HEADERS_DIR} ${IGOR_AUX_HEADERS_DIR} ${OMPF_HEADERS_DIR} ${MICA_HEADERS})
target_link_libraries(Mica Igor)

# OBJ2OMPF
set(OBJ2OMPF_HEADERS_DIR "${CMAKE_SOURCE_DIR}/src/Tools/OBJ2OMPF/src/")
file(GLOB_RECURSE OBJ2OMPF_SOURCES RELATIVE ${CMAKE_SOURCE_DIR} "src/Tools/OBJ2OMPF/src/*.c*")
file(GLOB_RECURSE OBJ2OMPF_HEADERS RELATIVE ${CMAKE_SOURCE_DIR} "src/Tools/OBJ2OMPF/src/*.h*")
add_executable(obj2ompf ${OBJ2OMPF_SOURCES} ${OBJ2OMPF_HEADERS} )
target_include_directories(obj2ompf PRIVATE  ${IGOR_HEADERS_DIR} ${IGOR_AUX_HEADERS_DIR} ${OMPF_HEADERS_DIR} ${OBJ2OMPF_HEADERS})
target_link_libraries(obj2ompf Igor)



# Examples
# currently don't work 99_Ascent 11_Instancing
foreach(EXAMPLE 00_FirstSteps 01_Keyboard 02_Mouse 03_2D 04_3D 05_Voxels 06_Particles 07_Physics 08_StateMachine 09_Widgets 10_LSystem 12_CharacterController 13_SpriteAnimation 14_Evaluation)
    message(${EXAMPLE})
    set(EXAMPLE_HEADERS_DIR "${CMAKE_SOURCE_DIR}/examples/${EXAMPLE}/src/")
    file(GLOB_RECURSE EXAMPLE_SOURCES RELATIVE ${CMAKE_SOURCE_DIR} "examples/${EXAMPLE}/src/*.c*")
    file(GLOB_RECURSE EXAMPLE_HEADERS RELATIVE ${CMAKE_SOURCE_DIR} "examples/${EXAMPLE}/src/*.h*")
    add_executable(${EXAMPLE} ${EXAMPLE_SOURCES} ${EXAMPLE_HEADERS} )
    target_include_directories(${EXAMPLE} PRIVATE  ${IGOR_HEADERS_DIR} ${IGOR_AUX_HEADERS_DIR} ${OMPF_HEADERS_DIR} ${EXAMPLE_HEADERS})
    target_link_libraries(${EXAMPLE} Igor)
endforeach()

###########
# Testing #
###########
# source of cmake gtest script https://crascit.com/2015/07/25/cmake-gtest/
# Download and unpack googletest at configure time
configure_file(CMakeLists.txt.inl googletest-download/CMakeLists.txt)
execute_process(COMMAND "${CMAKE_COMMAND}" -G "${CMAKE_GENERATOR}" .
    WORKING_DIRECTORY "${CMAKE_BINARY_DIR}/googletest-download"
)
execute_process(COMMAND "${CMAKE_COMMAND}" --build .
    WORKING_DIRECTORY "${CMAKE_BINARY_DIR}/googletest-download"
)

# Prevent GoogleTest from overriding our compiler/linker options
# when building with Visual Studio
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)

# Add googletest directly to our build. This adds the following targets:
# gtest, gtest_main, gmock and gmock_main
add_subdirectory("${CMAKE_BINARY_DIR}/googletest-src"
                 "${CMAKE_BINARY_DIR}/googletest-build"
)

# The gtest/gmock targets carry header search path dependencies
# automatically when using CMake 2.8.11 or later. Otherwise we
# have to add them here ourselves.
if(CMAKE_VERSION VERSION_LESS 2.8.11)
    include_directories("${gtest_SOURCE_DIR}/include"
                        "${gmock_SOURCE_DIR}/include"
    )
endif()

if(INSTALL_GTEST)
    message("Testing Enabled")
    enable_testing()
    set(IGOR_AUX_TEST_SRC_DIR "${CMAKE_SOURCE_DIR}/src/Tests/IgorAux/")
    set(IGOR_AUX_TEST_HEADERS_DIR ${IGOR_AUX_TEST_SRC_DIR})
    file(GLOB_RECURSE IGOR_AUX_TEST_SOURCES LIST_DIRECTORIES false "${IGOR_AUX_TEST_SRC_DIR}*.c*")
    file(GLOB_RECURSE IGOR_AUX_TEST_HEADERS LIST_DIRECTORIES false "${IGOR_AUX_TEST_SRC_DIR}*.h*")
    list(APPEND IGOR_AUX_TEST_SOURCES ${IGOR_AUX_TEST_HEADERS})
    add_executable(IgorAuxTest ${IGOR_AUX_TEST_SOURCES})
    if (MSVC_VERSION GREATER_EQUAL "1900")
        target_compile_options(IgorAuxTest PRIVATE "/std:c++latest" "/MP")
    endif()
    target_include_directories(IgorAuxTest PRIVATE  ${IGOR_AUX_TEST_HEADERS_DIR} ${IGOR_AUX_HEADERS_DIR} ${GTEST_INCLUDE_DIRS})

    # Now simply link your own targets against gtest, gmock,
    # etc. as appropriate
    target_link_libraries(IgorAuxTest IgorAux gtest)
    add_test(AllTestsInIgorAux IgorAuxTest)
endif()
