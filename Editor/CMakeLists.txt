cmake_minimum_required(VERSION 3.3)
project(Editor)

find_package(assimp REQUIRED)

# Gather c++ files
file(GLOB_RECURSE SRC_FILES 
    Source/*.cpp
    Source/*.h
)

include_directories (${SDL2_INCLUDE_DIRS})
include_directories (${assimp_INCLUDE_DIRS})
include_directories ("../Core/Source")
include_directories ("../Engine/Source")
include_directories ("../Networking/Source")
include_directories ("../Rendering/Source")
include_directories ("../Include/glm")
include_directories ("../Include/tinyxml")

add_executable(Editor ${SRC_FILES})

if(MSVC)
    target_compile_options(Editor PRIVATE /W3 /WX)
else()
    target_compile_options(Editor PRIVATE -Werror -Wall)
endif()

target_link_libraries(Editor Core)
target_link_libraries(Editor Engine)
target_link_libraries(Editor Networking)
target_link_libraries(Editor Rendering)

#Set working directory to the directory where "Resources" folder is located
set_target_properties(Editor PROPERTIES VS_DEBUGGER_WORKING_DIRECTORY "${CMAKE_SOURCE_DIR}")

# Set preprocessor definition MING3D_ENGINE_SOURCE_DIR (relative to CMAKE_SOURCE_DIR)
STRING(REGEX REPLACE "${CMAKE_SOURCE_DIR}/" "" EDITOR_SOURCE_DIR_REL ${CMAKE_CURRENT_SOURCE_DIR})
target_compile_definitions(Editor PRIVATE "MING3D_EDITOR_SOURCE_DIR=\"${EDITOR_SOURCE_DIR_REL}\"")
