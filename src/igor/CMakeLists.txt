cmake_minimum_required(VERSION 3.13)

message(STATUS "igor")

project(igor
    DESCRIPTION "Igor Game Engine core library" 
    LANGUAGES CXX)

# create igor version file
configure_file(${CMAKE_CURRENT_SOURCE_DIR}/igor/iVersion.cpp.inl ${CMAKE_CURRENT_BINARY_DIR}/iVersion.cpp)
set(IGOR_VERSION_FILE "${CMAKE_CURRENT_BINARY_DIR}/iVersion.cpp")
source_group("\\igor\\" FILES "${IGOR_VERSION_FILE}")

set(IGOR_SRC_DIR "${CMAKE_CURRENT_SOURCE_DIR}/igor/")
set(IGOR_HEADERS_DIR "${CMAKE_CURRENT_SOURCE_DIR}")

file(GLOB_RECURSE IGOR_SOURCES LIST_DIRECTORIES false "${IGOR_SRC_DIR}*.c*") #TODO
file(GLOB_RECURSE IGOR_HEADERS LIST_DIRECTORIES false "${IGOR_HEADERS_DIR}*.h*" "${IGOR_HEADERS_DIR}*.inl") #TODO
list(APPEND IGOR_SOURCES ${IGOR_HEADERS})
add_library(igor SHARED ${IGOR_SOURCES} ${TINYXML_SOURCES} ${STB_IMAGE_SOURCES} ${IGOR_VERSION_FILE} ${ENTT_SOURCES})

target_include_directories(igor
    PRIVATE 
    ${TINYXML_HEADERS_DIR}
    ${STB_IMAGE_HEADERS_DIR}
    ${ALSOFT_HEADERS_DIR}
    ${NEWTON_HEADERS_DIR}
    PUBLIC 
    ${IGOR_HEADERS_DIR}
    ${ENTT_HEADERS_DIR})
    
target_compile_features(igor PRIVATE cxx_std_17)

if(IS_GNU)
    target_compile_definitions(igor PRIVATE -D__IGOR_BUILDING_DLL__ ${X11_EXT_DEFINITIONS} -DENTT_USE_ATOMIC -D_NEWTON_USE_DOUBLE)    
    target_link_libraries(igor PUBLIC iaux TinyXML PRIVATE Glad ompf ${OPENGL_LIBRARIES} ${X11_EXT_LIBRARIES} OpenAL newton)
elseif(IS_MSVC)
    target_compile_definitions(igor PRIVATE -D__IGOR_BUILDING_DLL__ PUBLIC -DENTT_USE_ATOMIC)
    target_compile_options(igor PRIVATE "/MP" "/FS")
    target_link_libraries(igor PUBLIC iaux TinyXML PRIVATE Glad ompf OpenGL32 glu32 OpenAL newton)
endif()

set_property(TARGET igor PROPERTY FOLDER ${ENGINE_FOLDER})
source_group_special("${IGOR_SOURCES}" ${IGOR_SRC_DIR})

target_include_directories(igor PUBLIC .)

add_custom_target(igor_data ALL
    COMMAND ${CMAKE_COMMAND} -E copy_directory
    "${CMAKE_CURRENT_SOURCE_DIR}/data/"
    "${CMAKE_BINARY_DIR}/data")

add_dependencies(igor igor_data)
