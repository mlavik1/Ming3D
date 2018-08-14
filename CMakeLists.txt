cmake_minimum_required(VERSION 3.3)

set_property(DIRECTORY PROPERTY VS_STARTUP_PROJECT "Runtime" )

project(Ming3D)

set(CMAKE_BUILD_TYPE Debug)

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/CMake/")

set(BUILD_ROOT "${CMAKE_SOURCE_DIR}/build")
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_SOURCE_DIR}/build")

SET(LIB_DIR "${CMAKE_SOURCE_DIR}/Dependencies/lib/Windows/x86")
SET(ENGINE_LIB_DIR "${CMAKE_SOURCE_DIR}/build/Ming3D/Debug")

# Find includes in corresponding build directories
set(CMAKE_INCLUDE_CURRENT_DIR ON)

IF(WIN32)
	add_definitions(-DUNICODE)
endif()

# project
add_subdirectory(3rdParty)
add_subdirectory(Core)
add_subdirectory(Runtime)
add_subdirectory(Tests)
add_subdirectory(Networking)
add_subdirectory(Rendering)
add_subdirectory(Engine)
