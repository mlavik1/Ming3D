cmake_minimum_required(VERSION 3.3)
#project(Runtime)

# Gather c++ files
file(GLOB_RECURSE SRC_FILES 
	Source/*.cpp
	Source/*.h
)

include_directories ("../Core/Source")
include_directories ("../Engine/Source")

add_executable(Runtime ${SRC_FILES})

target_link_libraries(Runtime Engine)
