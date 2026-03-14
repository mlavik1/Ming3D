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

target_link_libraries(Engine PUBLIC Rendering)
target_link_libraries(Engine PUBLIC Core)
target_link_libraries(Engine PUBLIC Networking)
target_link_libraries(Engine PUBLIC SDL2::SDL2)
target_link_libraries(Engine PUBLIC SDL2_image::SDL2_image)
target_link_libraries(Engine PUBLIC assimp::assimp)
target_link_libraries(Engine PUBLIC PhysX::PhysX PhysX::PhysXExtensions)
target_compile_definitions(Engine PUBLIC PX_PHYSX_STATIC_LIB)
target_link_libraries(Engine PUBLIC Freetype::Freetype)
