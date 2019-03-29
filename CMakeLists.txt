cmake_minimum_required(VERSION 3.3)

set_property(DIRECTORY PROPERTY VS_STARTUP_PROJECT "Runtime" )

project(Ming3D)

set(CMAKE_BUILD_TYPE Debug)

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_CURRENT_SOURCE_DIR}/CMake/")

set(BUILD_ROOT "${CMAKE_CURRENT_BINARY_DIR}")
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_CURRENT_BINARY_DIR}")

set(EXTERNAL_LIB_DIR "${CMAKE_CURRENT_SOURCE_DIR}/ExternalLibraries" CACHE PATH "External library directory")

set(SDL_SEARCH_PATH ${EXTERNAL_LIB_DIR})

# Find includes in corresponding build directories
set(CMAKE_INCLUDE_CURRENT_DIR ON)

IF(WIN32)
	add_definitions(-DUNICODE)
endif()

set(MING3D_BUILD_EDITOR ON CACHE BOOL "Build editor project")
set(MING3D_BUILD_TESTS ON CACHE BOOL "Build test project")
set(MING3D_FORCE_OPENGL OFF CACHE BOOL "Use OpenGL as the preferred graphics API")

IF(MING3D_FORCE_OPENGL)
	add_definitions(-DMING3D_FORCE_OPENGL)
endif()

# project
add_subdirectory(3rdParty)
add_subdirectory(Core)
add_subdirectory(Runtime)
add_subdirectory(Networking)
add_subdirectory(Rendering)
add_subdirectory(Engine)

if(MING3D_BUILD_EDITOR)
	add_subdirectory(NativeUI)
	add_subdirectory(Editor)
endif()

if(MING3D_BUILD_TESTS)
	add_subdirectory(Tests)
endif()
