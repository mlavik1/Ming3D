cmake_minimum_required(VERSION 3.3)
project(Samples)

find_package(assimp REQUIRED)

# Gather c++ files
file(GLOB_RECURSE SRC_FILES 
	Source/*.cpp
	Source/*.h
)

set(SampleProject "game" CACHE STRING "Type of samples")
set_property(CACHE SampleProject PROPERTY STRINGS game sockets core rendering gamenetwork rpc replication physics multiwindow gui)

if(SampleProject STREQUAL "core")
	add_definitions(-DMING3D_TESTTYPE=1)
elseif(SampleProject STREQUAL "sockets")
	add_definitions(-DMING3D_TESTTYPE=2)
elseif(SampleProject STREQUAL "rendering")
	add_definitions(-DMING3D_TESTTYPE=3)
elseif(SampleProject STREQUAL "gamenetwork")
	add_definitions(-DMING3D_TESTTYPE=4)
elseif(SampleProject STREQUAL "rpc")
	add_definitions(-DMING3D_TESTTYPE=5)
elseif(SampleProject STREQUAL "replication")
	add_definitions(-DMING3D_TESTTYPE=6)
elseif(SampleProject STREQUAL "physics")
	add_definitions(-DMING3D_TESTTYPE=7)
elseif(SampleProject STREQUAL "game")
	add_definitions(-DMING3D_TESTTYPE=8)
elseif(SampleProject STREQUAL "multiwindow")
	add_definitions(-DMING3D_TESTTYPE=9)
    elseif(SampleProject STREQUAL "gui")
	add_definitions(-DMING3D_TESTTYPE=10)
endif()

include_directories (${SDL2_INCLUDE_DIRS})
include_directories (${assimp_INCLUDE_DIRS})
include_directories ("../Core/Source")
include_directories ("../Engine/Source")
include_directories ("../Networking/Source")
include_directories ("../Rendering/Source")
include_directories ("../Include/glm")
include_directories ("../Include/tinyxml")

add_executable(Samples ${SRC_FILES})

target_link_libraries(Samples Core)
target_link_libraries(Samples Engine)
target_link_libraries(Samples Networking)
target_link_libraries(Samples Rendering)

#Set working directory to the directory where "Resources" folder is located
set_target_properties(Samples PROPERTIES VS_DEBUGGER_WORKING_DIRECTORY "${CMAKE_SOURCE_DIR}")

# Set preprocessor definition MING3D_ENGINE_SOURCE_DIR (relative to CMAKE_SOURCE_DIR)
STRING(REGEX REPLACE "${CMAKE_SOURCE_DIR}/" "" SAMPLES_SOURCE_DIR_REL ${CMAKE_CURRENT_SOURCE_DIR})
target_compile_definitions(Samples PRIVATE "MING3D_SAMPLES_SOURCE_DIR=\"${SAMPLES_SOURCE_DIR_REL}\"")
