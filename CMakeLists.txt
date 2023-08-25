cmake_minimum_required(VERSION 3.13)

# set path to cmake modules
set(CMAKE_MODULE_PATH ${CMAKE_CURRENT_SOURCE_DIR}/cmake
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
project(igor DESCRIPTION "Igor Game Engine")

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
if(CMAKE_COMPILER_IS_GNUCXX)
    set(IS_GNU TRUE)
elseif(MSVC)
    set(IS_MSVC TRUE)
endif()

# setup some GNU specifics
if(IS_GNU)
    if(NOT CMAKE_BUILD_TYPE)
        message(STATUS "Setting build type to '${default_build_type}' as none was specified.")
        set(CMAKE_BUILD_TYPE "${default_build_type}" CACHE STRING "Choose the type of build." FORCE)
        # Set the possible values of build type for cmake-gui
        set_property(CACHE CMAKE_BUILD_TYPE PROPERTY STRINGS "Debug" "Release" "RelWithDebInfo")
    endif()

    # replicate the MSVC pre compiler definitions
    if(${CMAKE_BUILD_TYPE} STREQUAL "RelWithDebInfo")
        set(IGOR_GNU_BUILD_TYPE "-DRELWITHDEBINFO")
    elseif(${CMAKE_BUILD_TYPE} STREQUAL "Debug")
        set(IGOR_GNU_BUILD_TYPE "-DDEBUG")
    elseif(${CMAKE_BUILD_TYPE} STREQUAL "Release")
        set(IGOR_GNU_BUILD_TYPE "-DNDEBUG")
    endif()
endif()

# threading
set(THREADS_PREFER_PTHREAD_FLAG ON)
find_package(Threads REQUIRED)

# X11
if(LINUX)
  find_package(X11 REQUIRED)
  if(X11_xf86vmode_FOUND)
    set(X11_EXT_DEFINITIONS "-DXF86VMODE_FOUND")
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

set(NEWTON_DOUBLE_PRECISION ON CACHE BOOL "Newton: generate double precision" FORCE)
set(NEWTON_BUILD_CORE_ONLY ON CACHE BOOL "Newton: build core only" FORCE)
set(NEWTON_BUILD_SANDBOX_DEMOS OFF CACHE BOOL "Newton: build sandbox demos" FORCE)
add_subdirectory(vendor/newton-dynamics/newton-3.14)
set(NEWTON_HEADERS_DIR "vendor/newton-dynamics/newton-3.14/sdk")
# todo visual studio set_property(TARGET newton PROPERTY FOLDER ${VENDOR_FOLDER})

add_subdirectory(vendor/Glad)
set_property(TARGET Glad PROPERTY FOLDER ${VENDOR_FOLDER})

add_subdirectory(vendor/TinyXML)
set_property(TARGET TinyXML PROPERTY FOLDER ${VENDOR_FOLDER})           

# stb_image
message(STATUS "stb_image")
set(STB_IMAGE_SRC_DIR "${CMAKE_CURRENT_SOURCE_DIR}/vendor/stb_image/")
set(STB_IMAGE_HEADERS_DIR ${STB_IMAGE_SRC_DIR})
set(STB_IMAGE_HEADERS "${STB_IMAGE_HEADERS_DIR}stb_image.h")
list(APPEND STB_IMAGE_SOURCES ${STB_IMAGE_HEADERS})
source_group_special("${STB_IMAGE_SOURCES}" ${STB_IMAGE_SRC_DIR} "\\vendor\\stb_image\\")

# EnTT
message(STATUS "EnTT")
set(ENTT_SRC_DIR "${CMAKE_CURRENT_SOURCE_DIR}/vendor/entt/")
set(ENTT_HEADERS_DIR ${ENTT_SRC_DIR})
set(ENTT_HEADERS "${ENTT_HEADERS_DIR}entt.h")
list(APPEND ENTT_SOURCES ${ENTT_HEADERS})
source_group_special("${ENTT_SOURCES}" ${ENTT_SRC_DIR} "\\vendor\\EnTT\\")

add_subdirectory(src/iaux)
set_property(TARGET iaux PROPERTY FOLDER ${ENGINE_FOLDER})           

#####################
# OMPF
#####################
message(STATUS "ompf")

set(OMPF_SRC_DIR "${CMAKE_CURRENT_SOURCE_DIR}/src/ompf/")
set(OMPF_HEADERS_DIR ${OMPF_SRC_DIR})
file(GLOB_RECURSE OMPF_SOURCES LIST_DIRECTORIES false "${OMPF_SRC_DIR}*.c*") #TODO
file(GLOB_RECURSE OMPF_HEADERS LIST_DIRECTORIES false "${OMPF_HEADERS_DIR}*.h*" "${OMPF_HEADERS_DIR}*.inl") #TODO
list(APPEND OMPF_SOURCES ${OMPF_HEADERS})
add_library(ompf SHARED ${OMPF_SOURCES})

if(IS_GNU)
    target_compile_definitions(ompf PRIVATE -D__OMPF_BUILDING_DLL__)
    target_compile_options(ompf PRIVATE -std=c++17)
elseif(IS_MSVC)
    target_compile_definitions(ompf PRIVATE -D__OMPF_BUILDING_DLL__)
    target_compile_options(ompf PRIVATE "/std:c++17" "/MP" "/FS")
endif()

target_include_directories(ompf PUBLIC ${OMPF_HEADERS_DIR})
target_link_libraries(ompf PRIVATE iaux)

set_property(TARGET ompf PROPERTY FOLDER ${ENGINE_FOLDER})
source_group_special("${OMPF_SOURCES}" ${OMPF_SRC_DIR})

#####################
# Igor
#####################
message(STATUS "igor")

# create igor version file
configure_file(${CMAKE_CURRENT_SOURCE_DIR}/cmake/iVersion.cpp.inl ${CMAKE_CURRENT_BINARY_DIR}/iVersion.cpp)
set(IGOR_VERSION_FILE "${CMAKE_CURRENT_BINARY_DIR}/iVersion.cpp")
source_group("\\igor\\" FILES "${IGOR_VERSION_FILE}")

set(IGOR_SRC_DIR "${CMAKE_CURRENT_SOURCE_DIR}/src/igor/")
set(IGOR_HEADERS_DIR ${IGOR_SRC_DIR})

file(GLOB_RECURSE IGOR_SOURCES LIST_DIRECTORIES false "${IGOR_SRC_DIR}*.c*") #TODO
file(GLOB_RECURSE IGOR_HEADERS LIST_DIRECTORIES false "${IGOR_HEADERS_DIR}*.h*" "${IGOR_HEADERS_DIR}*.inl") #TODO
list(APPEND IGOR_SOURCES ${IGOR_HEADERS})
add_library(igor SHARED ${IGOR_SOURCES} ${TINYXML_SOURCES} ${STB_IMAGE_SOURCES} ${IGOR_VERSION_FILE} ${ENTT_SOURCES})

target_include_directories(igor
    PRIVATE 
    ${TINYXML_HEADERS_DIR}
    ${STB_IMAGE_HEADERS_DIR}
    ${ALSOFT_HEADERS_DIR}
    #"vendor/newton-dynamics/newton-4.00/sdk/dBrain"
    #"vendor/newton-dynamics/newton-4.00/sdk/dCollision"
    #"vendor/newton-dynamics/newton-4.00/sdk/dCore"
    #"vendor/newton-dynamics/newton-4.00/sdk/dFileFormat"
    #"vendor/newton-dynamics/newton-4.00/sdk/dModel"
    #"vendor/newton-dynamics/newton-4.00/sdk/dNewton"
    #"vendor/newton-dynamics/newton-4.00/sdk/dNewton/dExtensions"
    #"vendor/newton-dynamics/newton-4.00/sdk/dNewton/dIkSolver"
    #"vendor/newton-dynamics/newton-4.00/sdk/dNewton/dJoints"
    #"vendor/newton-dynamics/newton-4.00/sdk/dNewton/dModels"
    #"vendor/newton-dynamics/newton-4.00/sdk/dNewton/dModels/dVehicle"
    #"vendor/newton-dynamics/newton-4.00/sdk/dProfiler"
    ${NEWTON_HEADERS_DIR}
    PUBLIC 
    ${IGOR_HEADERS_DIR}
    ${ENTT_HEADERS_DIR})
    
if(IS_GNU)
    target_compile_definitions(igor PRIVATE -D__IGOR_BUILDING_DLL__ ${X11_EXT_DEFINITIONS} -DENTT_USE_ATOMIC -D_NEWTON_USE_DOUBLE)
    target_compile_options(igor PRIVATE -std=c++17)
    target_link_libraries(igor PUBLIC iaux TinyXML PRIVATE Glad ompf ${OPENGL_LIBRARIES} ${X11_EXT_LIBRARIES} OpenAL newton)
elseif(IS_MSVC)
    target_compile_definitions(igor PRIVATE -D__IGOR_BUILDING_DLL__ PUBLIC -DENTT_USE_ATOMIC)
    target_compile_options(igor PRIVATE "/std:c++17" "/MP" "/FS")
    target_link_libraries(igor PUBLIC iaux TinyXML PRIVATE Glad ompf OpenGL32 glu32 OpenAL newton)
endif()

set_property(TARGET igor PROPERTY FOLDER ${ENGINE_FOLDER})
source_group_special("${IGOR_SOURCES}" ${IGOR_SRC_DIR})

# MICA
message(STATUS "Mica")
set(MICA_SRC_DIR "${CMAKE_CURRENT_SOURCE_DIR}/src/tools/mica/src/")
set(MICA_HEADERS_DIR ${MICA_SRC_DIR})
file(GLOB_RECURSE MICA_SOURCES LIST_DIRECTORIES false "${MICA_SRC_DIR}*.c*") #TODO
file(GLOB_RECURSE MICA_HEADERS LIST_DIRECTORIES false "${MICA_HEADERS_DIR}*.h*" "${MICA_HEADERS_DIR}*.inl") #TODO
list(APPEND MICA_SOURCES ${MICA_HEADERS})
add_executable(mica ${MICA_SOURCES})

if(IS_GNU)
    target_compile_definitions(mica PRIVATE -D_cplusplus=201703L ${IGOR_GNU_BUILD_TYPE})
    target_compile_options(mica PRIVATE -std=c++17)
elseif(IS_MSVC)
    target_compile_definitions(mica PRIVATE -DNOMINMAX -D_UNICODE -DUNICODE)
    target_compile_options(mica PRIVATE "/std:c++17" "/MP" "/FS")
endif()

target_link_libraries(mica PRIVATE igor iaux ompf)
target_include_directories(mica PRIVATE ${IGOR_HEADERS_DIR})
set_property(TARGET mica PROPERTY FOLDER ${TOOLS_FOLDER})
source_group_special("${MICA_SOURCES}" ${MICA_SRC_DIR})

# OBJ2OMPF
set(OBJ2OMPF_SRC_DIR "${CMAKE_CURRENT_SOURCE_DIR}/src/tools/obj2ompf/src/")
set(OBJ2OMPF_HEADERS_DIR ${OBJ2OMPF_SRC_DIR})
file(GLOB_RECURSE OBJ2OMPF_SOURCES LIST_DIRECTORIES false "${OBJ2OMPF_SRC_DIR}*.c*") #TODO
file(GLOB_RECURSE OBJ2OMPF_HEADERS LIST_DIRECTORIES false "${OBJ2OMPF_HEADERS_DIR}*.h*" "${OBJ2OMPF_HEADERS_DIR}*.inl") #TODO
list(APPEND OBJ2OMPF_SOURCES ${OBJ2OMPF_HEADERS})
add_executable(obj2ompf ${OBJ2OMPF_SOURCES})

if(IS_GNU)
    target_compile_definitions(obj2ompf PRIVATE -D_cplusplus=201703L ${IGOR_GNU_BUILD_TYPE})
    target_compile_options(obj2ompf PRIVATE -std=c++17)
elseif(IS_MSVC)
    target_compile_definitions(obj2ompf PRIVATE -DNOMINMAX -D_UNICODE -DUNICODE)
    target_compile_options(obj2ompf PRIVATE "/std:c++17" "/MP" "/FS")
endif()

target_include_directories(obj2ompf PRIVATE ${IGOR_HEADERS_DIR} ${IAUX_HEADERS_DIR} ${OMPF_HEADERS_DIR})
target_link_libraries(obj2ompf PRIVATE igor)
set_property(TARGET obj2ompf PROPERTY FOLDER ${TOOLS_FOLDER})
source_group_special("${OBJ2OMPF_SOURCES}" ${OBJ2OMPF_SRC_DIR})

# Examples
set(EXAMPLE_BASE_HEADERS_DIR
    "${CMAKE_CURRENT_SOURCE_DIR}/examples/00_ExampleBase/src/")
file(GLOB_RECURSE EXAMPLE_BASE_SOURCES RELATIVE ${CMAKE_CURRENT_SOURCE_DIR} "examples/00_ExampleBase/src/*.c*") #TODO
file(GLOB_RECURSE EXAMPLE_BASE_HEADERS RELATIVE ${CMAKE_CURRENT_SOURCE_DIR} "examples/00_ExampleBase/src/*.h*") #TODO

foreach(
      EXAMPLE
      00_HelloWorld
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
      15_Sound
      98_Supremacy
      99_Ascent
      )

    message(STATUS "${EXAMPLE}")
    set(EXAMPLE_HEADERS_DIR "${CMAKE_CURRENT_SOURCE_DIR}/examples/${EXAMPLE}/src/")
    file(GLOB_RECURSE EXAMPLE_SOURCES RELATIVE ${CMAKE_CURRENT_SOURCE_DIR} "examples/${EXAMPLE}/src/*.c*") #TODO
    file(GLOB_RECURSE EXAMPLE_HEADERS RELATIVE ${CMAKE_CURRENT_SOURCE_DIR} "examples/${EXAMPLE}/src/*.h*") #TODO
    add_executable(${EXAMPLE} ${EXAMPLE_SOURCES} ${EXAMPLE_HEADERS} ${EXAMPLE_BASE_SOURCES} ${EXAMPLE_BASE_HEADERS})
    target_include_directories(${EXAMPLE} PRIVATE ${IGOR_HEADERS_DIR} ${IAUX_HEADERS_DIR} ${OMPF_HEADERS_DIR} ${EXAMPLE_BASE_HEADERS_DIR})
    target_link_libraries(${EXAMPLE} PRIVATE igor)

    if(IS_GNU)
        target_compile_definitions(${EXAMPLE} PRIVATE -D_cplusplus=201703L ${IGOR_GNU_BUILD_TYPE})
        target_compile_options(${EXAMPLE} PRIVATE -std=c++17)
    elseif(IS_MSVC)
        target_compile_definitions(${EXAMPLE} PRIVATE -DNOMINMAX -D_UNICODE -DUNICODE)
        target_compile_options(${EXAMPLE} PRIVATE "/std:c++17" "/MP" "/FS")
    endif()

    set_property(TARGET ${EXAMPLE} PROPERTY FOLDER ${EXAMPLES_FOLDER})
endforeach()

# iaux test
message(STATUS "iauxTest")
set(IAUX_TEST_SRC_DIR "${CMAKE_CURRENT_SOURCE_DIR}/src/tests/iaux/")
set(IAUX_TEST_HEADERS_DIR ${IAUX_TEST_SRC_DIR})
file(GLOB_RECURSE IAUX_TEST_SOURCES LIST_DIRECTORIES false "${IAUX_TEST_SRC_DIR}*.cpp") #TODO
list(APPEND IAUX_TEST_SOURCES ${IAUX_TEST_HEADERS})
add_executable(iauxTest ${IAUX_TEST_SOURCES})

if(IS_GNU)
    target_compile_definitions(iauxTest PRIVATE -D_cplusplus=201703L ${IGOR_GNU_BUILD_TYPE})
    target_compile_options(iauxTest PRIVATE -std=c++17)
elseif(IS_MSVC)
    target_compile_definitions(iauxTest PRIVATE -DNOMINMAX -D_UNICODE -DUNICODE)
    target_compile_options(iauxTest PRIVATE "/std:c++17" "/MP" "/FS")
endif()

target_include_directories(iauxTest PRIVATE ${IAUX_TEST_HEADERS_DIR} ${IAUX_HEADERS_DIR})
target_link_libraries(iauxTest PRIVATE iaux)
set_property(TARGET iauxTest PROPERTY FOLDER ${TESTING_FOLDER})

# Igor test
message(STATUS "igorTest")
set(IGOR_TEST_SRC_DIR "${CMAKE_CURRENT_SOURCE_DIR}/src/tests/igor/")
set(IGOR_TEST_HEADERS_DIR ${IGOR_TEST_SRC_DIR})
file(GLOB_RECURSE IGOR_TEST_SOURCES LIST_DIRECTORIES false "${IGOR_TEST_SRC_DIR}*.c*") #TODO
file(GLOB_RECURSE IGOR_TEST_HEADERS LIST_DIRECTORIES false "${IGOR_TEST_HEADERS_DIR}*.h*" "${IGOR_TEST_HEADERS_DIR}*.inl") #TODO
list(APPEND IGOR_TEST_SOURCES ${IGOR_TEST_HEADERS})
add_executable(igorTest ${IGOR_TEST_SOURCES})

if(IS_GNU)
    target_compile_definitions(igorTest PRIVATE -D_cplusplus=201703L ${IGOR_GNU_BUILD_TYPE})
    target_compile_options(igorTest PRIVATE -std=c++17)
elseif(IS_MSVC)
    target_compile_definitions(igorTest PRIVATE -DNOMINMAX -D_UNICODE -DUNICODE)
    target_compile_options(igorTest PRIVATE "/std:c++17" "/MP" "/FS")
endif()

target_include_directories(igorTest PRIVATE ${IGOR_TEST_HEADERS_DIR} ${IGOR_HEADERS_DIR})
target_link_libraries(igorTest PRIVATE igor)
set_property(TARGET igorTest PROPERTY FOLDER ${TESTING_FOLDER})
