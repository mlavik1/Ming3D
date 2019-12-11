cmake_minimum_required(VERSION 3.7)
project(Rendering)

# Gather c++ files
file(GLOB_RECURSE SRC_FILES
    Source/*.cpp
    Source/*.h
)

set(SourceDir Source)

add_library(Rendering STATIC ${SRC_FILES})

#list(APPEND CMAKE_PREFIX_PATH "E:/Libraries/SDL2-devel-all")

set(SDL2_DIR "${EXTERNAL_LIB_DIR}/SDL2-devel-all" CACHE PATH "Directory to search for SDL2 in")

# Find packages
find_package(SDL2 REQUIRED)
find_package(SDL2_image REQUIRED)
find_package(OpenGL REQUIRED)
find_package(GLEW)
if(NOT GLEW_FOUND)
    set(GLEW_SEARCH_PATH "${EXTERNAL_LIB_DIR}/glew-2.1.0" CACHE PATH "GLEW path")
    list(APPEND CMAKE_PREFIX_PATH "${GLEW_SEARCH_PATH}")
    if(${CMAKE_SYSTEM_NAME} MATCHES "Windows")
        if("${CMAKE_GENERATOR}" MATCHES "(Win64|IA64)")
            list(APPEND CMAKE_LIBRARY_PATH "${GLEW_SEARCH_PATH}/lib/Release/x64")
        else()
            list(APPEND CMAKE_LIBRARY_PATH "${GLEW_SEARCH_PATH}/lib/Release/Win32")
        endif()
    endif()
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
include_directories ("${BUILD_ROOT}/3rdParty/assimp/include") #TODO: Rendering project should not read files

# Link libraries
if(${CMAKE_SYSTEM_NAME} MATCHES "Windows")
    if("${CMAKE_GENERATOR}" MATCHES "(Win64|IA64)")
        target_link_libraries(Rendering d3d11.lib)
        target_link_libraries(Rendering d3dcompiler.lib)
    else()
        target_link_libraries(Rendering d3d11.lib)
        target_link_libraries(Rendering d3dcompiler.lib)
    endif()
endif()

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
    execute_process(COMMAND cmake -E copy ${SDL2_LIBRARY_DIRECTORY}/libjpeg-9.dll ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/Release/libjpeg-9.dll)
    execute_process(COMMAND cmake -E copy ${SDL2_LIBRARY_DIRECTORY}/libjpeg-9.dll ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/Debug/libjpeg-9.dll)
    execute_process(COMMAND cmake -E copy ${SDL2_LIBRARY_DIRECTORY}/libpng16-16.dll ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/Release/libpng16-16.dll)
    execute_process(COMMAND cmake -E copy ${SDL2_LIBRARY_DIRECTORY}/libpng16-16.dll ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/Debug/libpng16-16.dll)
    execute_process(COMMAND cmake -E copy ${SDL2_LIBRARY_DIRECTORY}/zlib1.dll ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/Release/zlib1.dll)
    execute_process(COMMAND cmake -E copy ${SDL2_LIBRARY_DIRECTORY}/zlib1.dll ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/Debug/zlib1.dll)

    execute_process(COMMAND cmake -E copy ${GLEW_BIN_DIRECTORY}/glew32.dll ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/Release/glew32.dll)
    execute_process(COMMAND cmake -E copy ${GLEW_BIN_DIRECTORY}/glew32.dll ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/Debug/glew32.dll)
endif()
