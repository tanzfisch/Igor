cmake_minimum_required(VERSION 3.13)

# set path to cmake modules
set(CMAKE_MODULE_PATH ${CMAKE_CURRENT_SOURCE_DIR}/src/cmake
                      ${CMAKE_MODULE_PATH})

# use folders for projects in visual studio solutions
set_property(GLOBAL PROPERTY USE_FOLDERS ON)

# include some modules
include(CreateMSVCFilters)
include(GetGitRevisionDescription)

# get git version tag
git_describe(VERSION --tags)
git_describe(VERSION_SHORT --abbrev=0 --tags)
message(STATUS "building version: ${VERSION_SHORT}")

# set up project
project(
  igor
  VERSION ${VERSION_SHORT}
  DESCRIPTION "Igor game engine. Made with joy and just for fun.")

# definitions
set(ENGINE_FOLDER "01 engine")
set(EXAMPLES_FOLDER "02 examples")
set(TOOLS_FOLDER "02 tools")
set(TESTING_FOLDER "03 testing")
set(VENDOR_FOLDER "04 vendor")
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
set(default_build_type "RelWithDebInfo")

# compiler
if(CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
  message(STATUS "using GCC")
  set(COMPILER "GCC")
elseif(MSVC_VERSION GREATER_EQUAL "1920")
  message(STATUS "using Visual Studio C++")
  set(COMPILER "MSVCXX")
elseif()
  message(FATAL_ERROR "unsupported compiler")
endif()

if(${COMPILER} STREQUAL "GCC")
  if(NOT CMAKE_BUILD_TYPE)
    message(STATUS "Setting build type to '${default_build_type}' as none was specified.")
    set(CMAKE_BUILD_TYPE "${default_build_type}" CACHE STRING "Choose the type of build." FORCE)
    # Set the possible values of build type for cmake-gui
    set_property(CACHE CMAKE_BUILD_TYPE PROPERTY STRINGS "Debug" "Release" "RelWithDebInfo")
  endif()

  if(${CMAKE_BUILD_TYPE} STREQUAL "RelWithDebInfo")
    add_definitions(-DRELWITHDEBINFO)
  endif()

  if(${CMAKE_BUILD_TYPE} STREQUAL "Debug")
    add_definitions(-DDEBUG)
  elseif()
    add_definitions(-DNDEBUG)
  endif()

  add_compile_options(-fpermissive -std=c++17)
  add_definitions(-D_NEWTON_USE_DOUBLE -D_cplusplus=201703L)

  if(DEEP_PROFILING)
    message(STATUS "Using Igor deep profiling") # what is that again?
    add_definitions(-DDEEP_PROFILING)
  endif()
endif()

if(${COMPILER} STREQUAL "MSVCXX")
  add_compile_options("/std:c++17" "/MP" "/W2")
  add_definitions(-DNOMINMAX -D_NEWTON_USE_DOUBLE)
endif()

if(WIN32)
  add_definitions(-D_UNICODE -DUNICODE)
endif()

# threading
set(THREADS_PREFER_PTHREAD_FLAG ON)
find_package(Threads REQUIRED)

# X11
if(LINUX)
  find_package(X11 REQUIRED)
  if(X11_xf86vmode_FOUND)
    add_definitions(-DXF86VMODE_FOUND)
    set(X11_EXT_LIBRARIES "Xxf86vm")
  else()
    message(WARNING "xf86vmode not available")  
  endif()
endif() # LINUX

# OpenGL
set(OpenGL_GL_PREFERENCE GLVND)
find_package(OpenGL REQUIRED)

# OpenAL Soft
if(LINUX)
  find_package(ALSA REQUIRED)
endif()
add_subdirectory(vendor/openal-soft)
set_property(TARGET alrecord PROPERTY FOLDER ${VENDOR_FOLDER})
set_property(TARGET altonegen PROPERTY FOLDER ${VENDOR_FOLDER})
set_property(TARGET build_version PROPERTY FOLDER ${VENDOR_FOLDER})
set_property(TARGET common PROPERTY FOLDER ${VENDOR_FOLDER})
set_property(TARGET ex-common PROPERTY FOLDER ${VENDOR_FOLDER})
set_property(TARGET OpenAL PROPERTY FOLDER ${VENDOR_FOLDER})
set_property(TARGET openal-info PROPERTY FOLDER ${VENDOR_FOLDER})

# tinyXML
message(STATUS "tinyXML")
set(TINYXML_SRC_DIR "${CMAKE_CURRENT_SOURCE_DIR}/vendor/TinyXML/")
set(TINYXML_HEADERS_DIR ${TINYXML_SRC_DIR})
set(TINYXML_SOURCES
    "${TINYXML_SRC_DIR}/tinystr.cpp" "${TINYXML_SRC_DIR}/tinyxml.cpp"
    "${TINYXML_SRC_DIR}/tinyxmlerror.cpp"
    "${TINYXML_SRC_DIR}/tinyxmlparser.cpp")
set(TINYXML_HEADERS "${TINYXML_HEADERS_DIR}/tinystr.h"
                    "${TINYXML_HEADERS_DIR}/tinyxml.h")
list(APPEND TINYXML_SOURCES ${TINYXML_HEADERS})
source_group_special("${TINYXML_SOURCES}" ${TINYXML_SRC_DIR}
                     "\\vendor\\TinyXML\\")

# stb_image
message(STATUS "stb_image")
set(STB_IMAGE_SRC_DIR "${CMAKE_CURRENT_SOURCE_DIR}/vendor/stb_image/")
set(STB_IMAGE_HEADERS_DIR ${STB_IMAGE_SRC_DIR})
set(STB_IMAGE_HEADERS "${STB_IMAGE_HEADERS_DIR}stb_image.h")
list(APPEND STB_IMAGE_SOURCES ${STB_IMAGE_HEADERS})
source_group_special("${STB_IMAGE_SOURCES}" ${STB_IMAGE_SRC_DIR}
                     "\\vendor\\stb_image\\")

# GLee
message(STATUS "GLee")
set(GLEE_SRC_DIR "${CMAKE_CURRENT_SOURCE_DIR}/vendor/GLee")
set(GLEE_HEADERS_DIR ${GLEE_SRC_DIR})
file(
  GLOB_RECURSE GLEE_SOURCES
  LIST_DIRECTORIES false
  "${GLEE_SRC_DIR}*.c*")
file(
  GLOB_RECURSE GLEE_HEADERS
  LIST_DIRECTORIES false
  "${GLEE_SRC_DIR}*.h*")
list(APPEND GLEE_SOURCES ${GLEE_HEADERS})
source_group_special("${GLEE_SOURCES}" ${GLEE_SRC_DIR} "\\vendor\\GLee\\")

# setup newton build
message(STATUS "newton")
set(TIME_TRACKER_HEADERS_DIR
    "${CMAKE_CURRENT_SOURCE_DIR}/vendor/newton-dynamics/packages/thirdParty/timeTracker/"
)
set(NEWTON_CORE_HEADERS_DIR
    "${CMAKE_CURRENT_SOURCE_DIR}/vendor/newton-dynamics/coreLibrary_300/source/core/"
)
set(NEWTON_PHYSICS_HEADERS_DIR
    "${CMAKE_CURRENT_SOURCE_DIR}/vendor/newton-dynamics/coreLibrary_300/source/physics/"
)
set(NEWTON_MESH_UTIL_HEADERS_DIR
    "${CMAKE_CURRENT_SOURCE_DIR}/vendor/newton-dynamics/coreLibrary_300/source/meshUtil/"
)
set(NEWTON_HEADERS_DIR
    "${CMAKE_CURRENT_SOURCE_DIR}/vendor/newton-dynamics/coreLibrary_300/source/"
)

file(
  GLOB NEWTON_CORE_SOURCES
  RELATIVE ${CMAKE_CURRENT_SOURCE_DIR}
  "vendor/newton-dynamics/coreLibrary_300/source/core/*.c*")
file(
  GLOB NEWTON_CORE_HEADERS
  RELATIVE ${CMAKE_CURRENT_SOURCE_DIR}
  "vendor/newton-dynamics/coreLibrary_300/source/core/*.h*")
add_library(core STATIC ${NEWTON_CORE_SOURCES} ${NEWTON_CORE_HEADERS})
if(${COMPILER} STREQUAL "GCC")
  target_compile_definitions(core PRIVATE -D_POSIX_VER_64)
  target_compile_options(
    core
    PRIVATE -fpic
            -msse
            -msse3
            -msse4
            -mfpmath=sse
            -ffloat-store
            -ffast-math
            -freciprocal-math
            -funsafe-math-optimizations
            -fsingle-precision-constant)
endif()
target_include_directories(core PRIVATE ${NEWTON_CORE_HEADERS_DIR}
                                        ${TIME_TRACKER_HEADERS_DIR})
set_property(TARGET core PROPERTY FOLDER ${VENDOR_FOLDER})

file(
  GLOB NEWTON_PHYSICS_SOURCES
  RELATIVE ${CMAKE_CURRENT_SOURCE_DIR}
  "vendor/newton-dynamics/coreLibrary_300/source/physics/*.c*")
file(
  GLOB NEWTON_PHYSICS_HEADERS
  RELATIVE ${CMAKE_CURRENT_SOURCE_DIR}
  "vendor/newton-dynamics/coreLibrary_300/source/physics/*.h*")
add_library(physics STATIC ${NEWTON_PHYSICS_SOURCES} ${NEWTON_PHYSICS_HEADERS})
if(${COMPILER} STREQUAL "GCC")
  target_compile_definitions(physics PRIVATE -D_POSIX_VER_64)
  target_compile_options(
    physics
    PRIVATE -fpic
            -msse
            -msse3
            -msse4
            -mfpmath=sse
            -ffloat-store
            -ffast-math
            -freciprocal-math
            -funsafe-math-optimizations
            -fsingle-precision-constant)
endif()
target_include_directories(
  physics PRIVATE ${NEWTON_PHYSICS_HEADERS_DIR} ${NEWTON_CORE_HEADERS_DIR}
                  ${TIME_TRACKER_HEADERS_DIR} ${NEWTON_MESH_UTIL_HEADERS_DIR})
set_property(TARGET physics PROPERTY FOLDER ${VENDOR_FOLDER})

file(
  GLOB NEWTON_MESH_UTIL_SOURCES
  RELATIVE ${CMAKE_CURRENT_SOURCE_DIR}
  "vendor/newton-dynamics/coreLibrary_300/source/meshUtil/*.c*")
file(
  GLOB NEWTON_MESH_UTIL_HEADERS
  RELATIVE ${CMAKE_CURRENT_SOURCE_DIR}
  "vendor/newton-dynamics/coreLibrary_300/source/meshUtil/*.h*")
add_library(meshUtil STATIC ${NEWTON_MESH_UTIL_SOURCES}
                            ${NEWTON_MESH_UTIL_HEADERS})
if(${COMPILER} STREQUAL "GCC")
  target_compile_definitions(meshUtil PRIVATE -D_POSIX_VER_64)
  target_compile_options(
    meshUtil
    PRIVATE -fpic
            -msse
            -msse3
            -msse4
            -mfpmath=sse
            -ffloat-store
            -ffast-math
            -freciprocal-math
            -funsafe-math-optimizations
            -fsingle-precision-constant)
endif()
target_include_directories(
  meshUtil PRIVATE ${NEWTON_MESH_UTIL_HEADERS_DIR} ${NEWTON_CORE_HEADERS_DIR}
                   ${TIME_TRACKER_HEADERS_DIR} ${NEWTON_PHYSICS_HEADERS_DIR})
set_property(TARGET meshUtil PROPERTY FOLDER ${VENDOR_FOLDER})

file(
  GLOB NEWTON_SOURCES
  RELATIVE ${CMAKE_CURRENT_SOURCE_DIR}
  "vendor/newton-dynamics/coreLibrary_300/source/newton/*.c*")
file(
  GLOB NEWTON_HEADERS
  RELATIVE ${CMAKE_CURRENT_SOURCE_DIR}
  "vendor/newton-dynamics/coreLibrary_300/source/newton/*.h*")
add_library(newton SHARED ${NEWTON_SOURCES} ${NEWTON_HEADERS})
if(${COMPILER} STREQUAL "GCC")
  target_compile_definitions(newton PRIVATE -D_POSIX_VER_64)
  target_compile_options(
    newton
    PRIVATE -fpic
            -msse
            -msse3
            -msse4
            -mfpmath=sse
            -ffloat-store
            -ffast-math
            -freciprocal-math
            -funsafe-math-optimizations
            -fsingle-precision-constant)
endif()
target_compile_definitions(newton PRIVATE -D_NEWTON_BUILD_DLL)
target_include_directories(
  newton
  PRIVATE ${NEWTON_HEADERS_DIR} ${NEWTON_MESH_UTIL_HEADERS_DIR}
          ${NEWTON_CORE_HEADERS_DIR} ${TIME_TRACKER_HEADERS_DIR}
          ${NEWTON_PHYSICS_HEADERS_DIR})
target_link_libraries(newton core physics meshUtil)
set_property(TARGET newton PROPERTY FOLDER ${VENDOR_FOLDER})

# iaux
message(STATUS "iaux")
set(IAUX_SRC_DIR "${CMAKE_CURRENT_SOURCE_DIR}/src/iaux/")
set(IAUX_HEADERS_DIR ${IAUX_SRC_DIR})
file(
  GLOB_RECURSE IAUX_SOURCES
  LIST_DIRECTORIES false
  "${IAUX_SRC_DIR}*.c*")
file(
  GLOB_RECURSE IAUX_HEADERS
  LIST_DIRECTORIES false
  "${IAUX_HEADERS_DIR}*.h*" "${IAUX_HEADERS_DIR}*.inl")
list(APPEND IAUX_SOURCES ${IAUX_HEADERS})

add_library(iaux SHARED ${IAUX_SOURCES})
source_group_special("${IAUX_SOURCES}" ${IAUX_SRC_DIR})

target_compile_definitions(iaux PRIVATE -D__IAUX_BUILDING_DLL__)
target_include_directories(iaux PUBLIC ${IAUX_HEADERS_DIR})

if(${COMPILER} STREQUAL "GCC")
  target_link_libraries(iaux PRIVATE stdc++fs Threads::Threads)
elseif(${COMPILER} STREQUAL "MSVCXX")
  target_link_libraries(iaux PRIVATE DbgHelp Threads::Threads)
endif()

set_property(TARGET iaux PROPERTY FOLDER ${ENGINE_FOLDER})

# OMPF
message(STATUS "ompf")

set(OMPF_SRC_DIR "${CMAKE_CURRENT_SOURCE_DIR}/src/ompf/")
set(OMPF_HEADERS_DIR ${OMPF_SRC_DIR})

file(
  GLOB_RECURSE OMPF_SOURCES
  LIST_DIRECTORIES false
  "${OMPF_SRC_DIR}*.c*")
file(
  GLOB_RECURSE OMPF_HEADERS
  LIST_DIRECTORIES false
  "${OMPF_HEADERS_DIR}*.h*" "${OMPF_HEADERS_DIR}*.inl")
list(APPEND OMPF_SOURCES ${OMPF_HEADERS})

add_library(ompf SHARED ${OMPF_SOURCES})
source_group_special("${OMPF_SOURCES}" ${OMPF_SRC_DIR})

target_compile_definitions(ompf PRIVATE -D__OMPF_BUILDING_DLL__)
target_include_directories(ompf PUBLIC ${OMPF_HEADERS_DIR})
target_link_libraries(ompf iaux)

set_property(TARGET ompf PROPERTY FOLDER ${ENGINE_FOLDER})

# Igor
message(STATUS "igor")

# create igor version file
configure_file(${CMAKE_CURRENT_SOURCE_DIR}/src/cmake/iVersion.cpp.inl
               ${CMAKE_CURRENT_BINARY_DIR}/iVersion.cpp)
set(IGOR_VERSION_FILE "${CMAKE_CURRENT_BINARY_DIR}/iVersion.cpp")
source_group("\\igor\\" FILES "${IGOR_VERSION_FILE}")

set(IGOR_SRC_DIR "${CMAKE_CURRENT_SOURCE_DIR}/src/igor/")
set(IGOR_HEADERS_DIR ${IGOR_SRC_DIR})

file(
  GLOB_RECURSE IGOR_SOURCES
  LIST_DIRECTORIES false
  "${IGOR_SRC_DIR}*.c*")
file(
  GLOB_RECURSE IGOR_HEADERS
  LIST_DIRECTORIES false
  "${IGOR_HEADERS_DIR}*.h*" "${IGOR_HEADERS_DIR}*.inl")
list(APPEND IGOR_SOURCES ${IGOR_HEADERS})

add_library(igor SHARED ${IGOR_SOURCES} ${GLEE_SOURCES} ${TINYXML_SOURCES}
                        ${STB_IMAGE_SOURCES} ${IGOR_VERSION_FILE})

source_group_special("${IGOR_SOURCES}" ${IGOR_SRC_DIR})

target_include_directories(
  igor
  PRIVATE ${GLEE_HEADERS_DIR}
          ${TINYXML_HEADERS_DIR}
          ${STB_IMAGE_HEADERS_DIR}
          ${NEWTON_PHYSICS_HEADERS_DIR}
          ${NEWTON_CORE_HEADERS_DIR}
          ${NEWTON_HEADERS_DIR}
          ${ALSOFT_HEADERS_DIR}
  PUBLIC ${IGOR_HEADERS_DIR})

if(${COMPILER} STREQUAL "GCC")
  target_link_libraries(
    igor
    PUBLIC iaux
    PRIVATE ompf newton ${OPENGL_LIBRARIES} ${X11_EXT_LIBRARIES} OpenAL)
  target_compile_definitions(igor PRIVATE -D__IGOR_BUILDING_DLL__)
elseif(${COMPILER} STREQUAL "MSVCXX")
  target_link_libraries(
    igor
    PUBLIC iaux
    PRIVATE ompf OpenGL32 glu32 newton OpenAL)
  target_compile_definitions(igor PRIVATE -D__IGOR_BUILDING_DLL__)
endif()

set_property(TARGET igor PROPERTY FOLDER ${ENGINE_FOLDER})

# MICA
message(STATUS "Mica")
set(MICA_SRC_DIR "${CMAKE_CURRENT_SOURCE_DIR}/src/tools/mica/src/")
set(MICA_HEADERS_DIR ${MICA_SRC_DIR})
file(
  GLOB_RECURSE MICA_SOURCES
  LIST_DIRECTORIES false
  "${MICA_SRC_DIR}*.c*")
file(
  GLOB_RECURSE MICA_HEADERS
  LIST_DIRECTORIES false
  "${MICA_HEADERS_DIR}*.h*" "${MICA_HEADERS_DIR}*.inl")
list(APPEND MICA_SOURCES ${MICA_HEADERS})
add_executable(mica ${MICA_SOURCES})
source_group_special("${MICA_SOURCES}" ${MICA_SRC_DIR})
target_include_directories(mica PRIVATE ${IGOR_HEADERS_DIR} ${IAUX_HEADERS_DIR}
                                        ${OMPF_HEADERS_DIR})
target_link_libraries(mica igor)
set_property(TARGET mica PROPERTY FOLDER ${TOOLS_FOLDER})

# OBJ2OMPF
set(OBJ2OMPF_SRC_DIR "${CMAKE_CURRENT_SOURCE_DIR}/src/tools/obj2ompf/src/")
set(OBJ2OMPF_HEADERS_DIR ${OBJ2OMPF_SRC_DIR})
file(
  GLOB_RECURSE OBJ2OMPF_SOURCES
  LIST_DIRECTORIES false
  "${OBJ2OMPF_SRC_DIR}*.c*")
file(
  GLOB_RECURSE OBJ2OMPF_HEADERS
  LIST_DIRECTORIES false
  "${OBJ2OMPF_HEADERS_DIR}*.h*" "${OBJ2OMPF_HEADERS_DIR}*.inl")
list(APPEND OBJ2OMPF_SOURCES ${OBJ2OMPF_HEADERS})
add_executable(obj2ompf ${OBJ2OMPF_SOURCES})
source_group_special("${OBJ2OMPF_SOURCES}" ${OBJ2OMPF_SRC_DIR})
target_include_directories(
  obj2ompf PRIVATE ${IGOR_HEADERS_DIR} ${IAUX_HEADERS_DIR} ${OMPF_HEADERS_DIR})
target_link_libraries(obj2ompf igor)
set_property(TARGET obj2ompf PROPERTY FOLDER ${TOOLS_FOLDER})

# Examples
set(EXAMPLE_BASE_HEADERS_DIR
    "${CMAKE_CURRENT_SOURCE_DIR}/examples/00_ExampleBase/src/")
file(
  GLOB_RECURSE EXAMPLE_BASE_SOURCES
  RELATIVE ${CMAKE_CURRENT_SOURCE_DIR}
  "examples/00_ExampleBase/src/*.c*")
file(
  GLOB_RECURSE EXAMPLE_BASE_HEADERS
  RELATIVE ${CMAKE_CURRENT_SOURCE_DIR}
  "examples/00_ExampleBase/src/*.h*")

foreach(
  EXAMPLE
  00_FirstSteps
  01_Keyboard
  02_Mouse
  03_2D
  04_3D
  05_Voxels
  06_Particles
  07_Physics
  09_Widgets
  10_LSystem
  11_Instancing
  12_CharacterController
  13_SpriteAnimation
  14_Evaluation
  15_Sound
  98_Supremacy
  99_Ascent)

  message(STATUS "${EXAMPLE}")
  set(EXAMPLE_HEADERS_DIR
      "${CMAKE_CURRENT_SOURCE_DIR}/examples/${EXAMPLE}/src/")
  file(
    GLOB_RECURSE EXAMPLE_SOURCES
    RELATIVE ${CMAKE_CURRENT_SOURCE_DIR}
    "examples/${EXAMPLE}/src/*.c*")
  file(
    GLOB_RECURSE EXAMPLE_HEADERS
    RELATIVE ${CMAKE_CURRENT_SOURCE_DIR}
    "examples/${EXAMPLE}/src/*.h*")
  add_executable(${EXAMPLE} ${EXAMPLE_SOURCES} ${EXAMPLE_HEADERS}
                            ${EXAMPLE_BASE_SOURCES} ${EXAMPLE_BASE_HEADERS})
  target_include_directories(
    ${EXAMPLE} PRIVATE ${IGOR_HEADERS_DIR} ${IAUX_HEADERS_DIR}
                       ${OMPF_HEADERS_DIR} ${EXAMPLE_BASE_HEADERS_DIR})
  target_link_libraries(${EXAMPLE} igor)

  set_property(TARGET ${EXAMPLE} PROPERTY FOLDER ${EXAMPLES_FOLDER})
endforeach()

# ##############################################################################
# Testing #
# ##############################################################################
message(STATUS "iauxTest")
set(IAUX_TEST_SRC_DIR "${CMAKE_CURRENT_SOURCE_DIR}/src/tests/iaux/")
set(IAUX_TEST_HEADERS_DIR ${IAUX_TEST_SRC_DIR})
file(
  GLOB_RECURSE IAUX_TEST_SOURCES
  LIST_DIRECTORIES false
  "${IAUX_TEST_SRC_DIR}*.c*")
file(
  GLOB_RECURSE IAUX_TEST_HEADERS
  LIST_DIRECTORIES false
  "${IAUX_TEST_HEADERS_DIR}*.h*" "${IAUX_TEST_HEADERS_DIR}*.inl")
list(APPEND IAUX_TEST_SOURCES ${IAUX_TEST_HEADERS})
add_executable(iauxTest ${IAUX_TEST_SOURCES})
target_include_directories(iauxTest PRIVATE ${IAUX_TEST_HEADERS_DIR}
                                            ${IAUX_HEADERS_DIR})

target_link_libraries(iauxTest iaux)
set_property(TARGET iauxTest PROPERTY FOLDER ${TESTING_FOLDER})

message(STATUS "igorTest")
set(IGOR_TEST_SRC_DIR "${CMAKE_CURRENT_SOURCE_DIR}/src/tests/igor/")
set(IGOR_TEST_HEADERS_DIR ${IGOR_TEST_SRC_DIR})
file(
  GLOB_RECURSE IGOR_TEST_SOURCES
  LIST_DIRECTORIES false
  "${IGOR_TEST_SRC_DIR}*.c*")
file(
  GLOB_RECURSE IGOR_TEST_HEADERS
  LIST_DIRECTORIES false
  "${IGOR_TEST_HEADERS_DIR}*.h*" "${IGOR_TEST_HEADERS_DIR}*.inl")
list(APPEND IGOR_TEST_SOURCES ${IGOR_TEST_HEADERS})
add_executable(igorTest ${IGOR_TEST_SOURCES})
target_include_directories(igorTest PRIVATE ${IGOR_TEST_HEADERS_DIR}
                                            ${IGOR_HEADERS_DIR})

target_link_libraries(igorTest igor)
set_property(TARGET igorTest PROPERTY FOLDER ${TESTING_FOLDER})
