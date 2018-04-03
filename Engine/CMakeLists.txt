cmake_minimum_required(VERSION 3.3)

# Gather c++ files
file(GLOB_RECURSE SRC_FILES 
	Source/*.cpp
	Source/*.h
)

set(SourceDir ${PROJECT_SOURCE_DIR}/Source)

include_directories ("../Core/Source")

add_library(Engine STATIC ${SRC_FILES})

target_link_libraries(Engine Core)
