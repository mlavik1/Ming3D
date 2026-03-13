cmake_minimum_required(VERSION 3.7)
project(Rendering)

# Gather c++ files
file(GLOB_RECURSE SRC_FILES
    Source/*.cpp
    Source/*.h
)

set(SourceDir Source)

add_library(Rendering STATIC ${SRC_FILES})

if(MSVC)
    target_compile_options(Rendering PRIVATE /W3 /W4)
else()
    target_compile_options(Rendering PRIVATE -Werror -Wall)
endif()

# Find packages
find_package(SDL2 REQUIRED)
find_package(SDL2_image REQUIRED)
find_package(OpenGL REQUIRED)
find_package(GLEW REQUIRED CONFIG)
find_package(assimp REQUIRED)

# Include
include_directories(${SourceDir})
include_directories ("../Include/glm")
include_directories("../Core/Source")

# Link libraries
if(${CMAKE_SYSTEM_NAME} MATCHES "Windows")
    target_link_libraries(Rendering d3d11.lib)
    target_link_libraries(Rendering d3dcompiler.lib)
endif()

target_link_libraries(Rendering SDL2::SDL2)
target_link_libraries(Rendering SDL2_image::SDL2_image)
target_link_libraries(Rendering OpenGL::GL)
target_link_libraries(Rendering GLEW::GLEW)
target_link_libraries(Rendering assimp::assimp) #TODO: Rendering project should not read files. Instead, read in engine and send data to renderer.
