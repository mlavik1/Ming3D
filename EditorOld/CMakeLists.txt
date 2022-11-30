cmake_minimum_required(VERSION 3.3)
project(Editor)

# Gather c++ files
file(GLOB_RECURSE SRC_FILES 
	Source/*.cpp
	Source/*.h
)

add_definitions(-DUNICODE)
add_definitions(-D_UNICODE)

include_directories ("../Core/Source")
include_directories ("../Engine/Source")
include_directories ("../Rendering/Source")
include_directories ("../NativeUI/Source")
include_directories ("../Include/glm")

add_executable(Editor ${SRC_FILES})

ADD_DEPENDENCIES(Editor Engine)
ADD_DEPENDENCIES(Editor Rendering)
ADD_DEPENDENCIES(Editor NativeUI)

target_link_libraries(Editor Engine)
target_link_libraries(Editor Rendering)
target_link_libraries(Editor NativeUI)

#Set working directory to the directory where "Resources" folder is located
set_target_properties(Editor PROPERTIES VS_DEBUGGER_WORKING_DIRECTORY "${CMAKE_SOURCE_DIR}")
