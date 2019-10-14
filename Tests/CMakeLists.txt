cmake_minimum_required(VERSION 3.3)
project(Tests)

# Gather c++ files
file(GLOB_RECURSE SRC_FILES 
	Source/*.cpp
	Source/*.h
)

set(TestType "sockets" CACHE STRING "Type of test")
set_property(CACHE TestType PROPERTY STRINGS sockets core rendering gamenetwork rpc replication physics)

if(TestType STREQUAL "core")
	add_definitions(-DMING3D_TESTTYPE=1)
elseif(TestType STREQUAL "sockets")
	add_definitions(-DMING3D_TESTTYPE=2)
elseif(TestType STREQUAL "rendering")
	add_definitions(-DMING3D_TESTTYPE=3)
elseif(TestType STREQUAL "gamenetwork")
	add_definitions(-DMING3D_TESTTYPE=4)
elseif(TestType STREQUAL "rpc")
	add_definitions(-DMING3D_TESTTYPE=5)
elseif(TestType STREQUAL "replication")
	add_definitions(-DMING3D_TESTTYPE=6)
elseif(TestType STREQUAL "physics")
	add_definitions(-DMING3D_TESTTYPE=7)
endif()

include_directories ("../Core/Source")
include_directories ("../Engine/Source")
include_directories ("../Networking/Source")
include_directories ("../Rendering/Source")
include_directories ("../Include/glm")

add_executable(Tests ${SRC_FILES})

target_link_libraries(Tests Core)
target_link_libraries(Tests Engine)
target_link_libraries(Tests Networking)
target_link_libraries(Tests Rendering)

#Set working directory to the directory where "Resources" folder is located
set_target_properties(Tests PROPERTIES VS_DEBUGGER_WORKING_DIRECTORY "${CMAKE_SOURCE_DIR}")
