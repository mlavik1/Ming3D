cmake_minimum_required(VERSION 3.3)

# Gather c++ files
file(GLOB_RECURSE SRC_FILES 
	Source/*.cpp
	Source/*.h
)

set(SourceDir $Source)

include_directories (${SourceDir})
include_directories ("Source")
include_directories ("../Core/Source")
include_directories ("../Rendering/Source")
include_directories ("../Networking/Source")
include_directories ("../Include/glm")
include_directories (${SDL2_INCLUDE_DIR})
include_directories ("../3rdParty/assimp/include")
include_directories ("${CMAKE_BINARY_DIR}/3rdParty/assimp/include")

add_library(Engine STATIC ${SRC_FILES})

target_link_libraries(Engine Rendering)
target_link_libraries(Engine Core)
target_link_libraries(Engine Networking)
target_link_libraries(Engine ${SDL2_LIBRARIES})
target_link_libraries(Engine assimp)
