cmake_minimum_required(VERSION 3.3)

# Gather c++ files
file(GLOB_RECURSE SRC_FILES 
	Source/*.cpp
	Source/*.h
)

set(SourceDir ${PROJECT_SOURCE_DIR}/Source)

add_library(Networking STATIC ${SRC_FILES})
