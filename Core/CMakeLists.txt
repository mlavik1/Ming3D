cmake_minimum_required(VERSION 3.3)
project(Core)

# Gather c++ files
file(GLOB_RECURSE SRC_FILES 
	Source/*.cpp
	Source/*.h
)

set(SourceDir Source)

file(GLOB_RECURSE SRC_DEBUG
    "${SourceDir}/Debug/*.cpp"
    "${SourceDir}/Debug/*.h"
)

file(GLOB_RECURSE SRC_OBJECT
    "${SourceDir}/Object/*.cpp"
    "${SourceDir}/Object/*.h"
)

file(GLOB_RECURSE SRC_SERIALISATION
    "${SourceDir}/Serialisation/*.cpp"
    "${SourceDir}/Serialisation/*.h"
)

source_group("Debug" FILES ${SRC_DEBUG})
source_group("Object" FILES ${SRC_OBJECT})
source_group("Serialisation" FILES ${SRC_SERIALISATION})

include_directories (${SourceDir})
include_directories ("Include")

add_library(Core STATIC ${SRC_FILES})
