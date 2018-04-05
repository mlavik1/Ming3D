cmake_minimum_required(VERSION 3.7)

# Gather c++ files
file(GLOB_RECURSE SRC_FILES 
	Source/*.cpp
	Source/*.h
)

set(SourceDir Source)

add_library(Rendering STATIC ${SRC_FILES})

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/CMake/")

#list(APPEND CMAKE_PREFIX_PATH "E:/Libraries/SDL2-devel-all")

# Find packages
find_package(SDL2 REQUIRED)
find_package(SDL2_image REQUIRED)
find_package(OpenGL REQUIRED)
find_package(GLEW)
if(NOT GLEW_FOUND)
	set(GLEW_SEARCH_PATH "" CACHE PATH "GLEW path")
	list(APPEND CMAKE_PREFIX_PATH "${GLEW_SEARCH_PATH}")
	list(APPEND CMAKE_LIBRARY_PATH "${GLEW_SEARCH_PATH}/lib/Release/Win32")
	find_package(GLEW REQUIRED)
endif()

if(GLEW_FOUND)
	get_filename_component(GLEW_BIN_DIRECTORY ${GLEW_LIBRARIES} DIRECTORY)
	string(REPLACE "/lib/" "/bin/" GLEW_BIN_DIRECTORY ${GLEW_BIN_DIRECTORY})
endif()

# Include
include_directories (${SourceDir})
include_directories ("../Core/Source")
include_directories ("../Include/glm")
include_directories(${SDL2_INCLUDE_DIR})
include_directories(${GLEW_INCLUDE_DIRS})
include_directories ("../3rdParty/assimp/include") #TODO: Rendering project should not read files
include_directories ("${CMAKE_BINARY_DIR}/3rdParty/assimp/include") #TODO: Rendering project should not read files

# Link libraries
target_link_libraries(Rendering ${SDL2_LIBRARIES})
target_link_libraries(Rendering ${SDL2IMAGE_LIBRARY})
target_link_libraries(Rendering ${OPENGL_gl_LIBRARY})
target_link_libraries(Rendering GLEW::GLEW)
target_link_libraries(Rendering assimp) #TODO: Rendering project should not read files. Instead, read in engine and send data to renderer.

# Copy DLLs	
if(${CMAKE_SYSTEM_NAME} MATCHES "Windows")
	execute_process(COMMAND cmake -E copy ${SDL2_LIBRARY_DIRECTORY}/SDL2.dll ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/Release/SDL2.dll)
	execute_process(COMMAND cmake -E copy ${SDL2_LIBRARY_DIRECTORY}/SDL2.dll ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/Debug/SDL2.dll)
	execute_process(COMMAND cmake -E copy ${SDL2_LIBRARY_DIRECTORY}/SDL2_image.dll ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/Release/SDL2_image.dll)
	execute_process(COMMAND cmake -E copy ${SDL2_LIBRARY_DIRECTORY}/SDL2_image.dll ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/Debug/SDL2_image.dll)

	execute_process(COMMAND cmake -E copy ${GLEW_BIN_DIRECTORY}/glew32.dll ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/Release/glew32.dll)
	execute_process(COMMAND cmake -E copy ${GLEW_BIN_DIRECTORY}/glew32.dll ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/Debug/glew32.dll)
endif()
