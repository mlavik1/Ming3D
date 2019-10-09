cmake_minimum_required(VERSION 3.3)
#project(Viewer)

# Gather c++ files
file(GLOB_RECURSE SRC_FILES 
	Source/*.cpp
	Source/*.h
)

include_directories ("../Core/Source")
include_directories ("../Engine/Source")
include_directories ("../Include/glm")

add_executable(Viewer ${SRC_FILES})

target_link_libraries(Viewer Engine)

#Set working directory to the directory where "Resources" folder is located
set_target_properties(Viewer PROPERTIES VS_DEBUGGER_WORKING_DIRECTORY "${CMAKE_SOURCE_DIR}")
