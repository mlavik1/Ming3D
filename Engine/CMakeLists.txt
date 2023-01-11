cmake_minimum_required(VERSION 3.3)
project(Engine)

find_package(assimp REQUIRED)
find_package(SDL2 REQUIRED)
find_package(SDL2_image REQUIRED)
find_package(PhysX REQUIRED)
find_package(Freetype REQUIRED)

# Gather c++ files
file(GLOB_RECURSE SRC_FILES 
    Source/*.cpp
    Source/*.h
)

include_directories ("Source")
include_directories (${SDL2_INCLUDE_DIRS})
include_directories (${SDL2_image_INCLUDE_DIRS})
include_directories (${assimp_INCLUDE_DIRS})
include_directories (${PhysX_INCLUDE_DIRS})
include_directories (${Freetype_INCLUDE_DIRS})
include_directories ("../Core/Source")
include_directories ("../Rendering/Source")
include_directories ("../Networking/Source")
include_directories ("../Include/glm")

add_library(Engine STATIC ${SRC_FILES})

if(MSVC)
    target_compile_options(Engine PRIVATE /W3 /WX)
else()
    target_compile_options(Engine PRIVATE -Werror -Wall)
endif()

# Set preprocessor definition MING3D_ENGINE_SOURCE_DIR (relative to CMAKE_SOURCE_DIR)
STRING(REGEX REPLACE "${CMAKE_SOURCE_DIR}/" "" ENGINE_SOURCE_DIR_REL ${CMAKE_CURRENT_SOURCE_DIR})
target_compile_definitions(Engine PRIVATE "MING3D_ENGINE_SOURCE_DIR=\"${ENGINE_SOURCE_DIR_REL}\"")

target_link_libraries(Engine Rendering)
target_link_libraries(Engine Core)
target_link_libraries(Engine Networking)
target_link_libraries(Engine ${SDL2_LIBRARIES})
target_link_libraries(Engine ${SDL2_image_LIBRARIES})
target_link_libraries(Engine ${assimp_LIBRARIES})
target_link_libraries(Engine ${PhysX_LIBRARIES})
target_link_libraries(Engine ${Freetype_LIBRARIES})
