cmake_minimum_required(VERSION 3.7)
project(Rendering)

# Gather c++ files
file(GLOB_RECURSE SRC_FILES
    Source/*.cpp
    Source/*.h
)

set(SourceDir Source)

add_library(Rendering STATIC ${SRC_FILES})

target_compile_options(Rendering PRIVATE -Werror -Wall)

# Find packages
find_package(SDL2 REQUIRED)
find_package(SDL2_image REQUIRED)
find_package(OpenGL REQUIRED)
find_package(GLEW REQUIRED)

# Include
include_directories(${SourceDir})
include_directories(${SDL2_INCLUDE_DIRS})
include_directories(${sdl2_image_INCLUDE_DIRS})
include_directories(${GLEW_INCLUDE_DIRS})
include_directories ("../Include/glm")
include_directories("../Core/Source")

# Link libraries
if(${CMAKE_SYSTEM_NAME} MATCHES "Windows")
    target_link_libraries(Rendering d3d11.lib)
    target_link_libraries(Rendering d3dcompiler.lib)
endif()

target_link_libraries(Rendering ${SDL2_LIBRARIES})
target_link_libraries(Rendering ${sdl2_image_LIBRARIES})
target_link_libraries(Rendering ${opengl_LIBRARIES})
target_link_libraries(Rendering ${GLEW_LIBRARIES})
target_link_libraries(Rendering ${assimp_LIBRARIES}) #TODO: Rendering project should not read files. Instead, read in engine and send data to renderer.
