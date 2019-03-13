cmake_minimum_required(VERSION 3.3)

# Gather c++ files
file(GLOB_RECURSE SRC_FILES 
	Source/*.cpp
	Source/*.h
)

set(PHYSX_INC_DIR "${EXTERNAL_LIB_DIR}/physx/include")
set(PHYSX_LIB_DIR "${EXTERNAL_LIB_DIR}/physx/lib")

include_directories ("Source")
include_directories (${PHYSX_INC_DIR})
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
target_link_libraries(Engine assimp)

target_link_libraries(Engine debug ${PHYSX_LIB_DIR}/vc14-win32/Debug/PhysX3DEBUG_x86.lib)
target_link_libraries(Engine debug ${PHYSX_LIB_DIR}/vc14-win32/Debug/PhysX3CommonDEBUG_x86.lib)
target_link_libraries(Engine debug ${PHYSX_LIB_DIR}/vc14-win32/Debug/PhysX3ExtensionsDEBUG.lib)
target_link_libraries(Engine debug ${PHYSX_LIB_DIR}/vc14-win32/Debug/PxFoundationDEBUG_x86.lib)
target_link_libraries(Engine optimized ${PHYSX_LIB_DIR}/vc14-win32/Release/PhysX3_x86.lib)
target_link_libraries(Engine optimized ${PHYSX_LIB_DIR}/vc14-win32/Release/PhysX3Common_x86.lib)
target_link_libraries(Engine optimized ${PHYSX_LIB_DIR}/vc14-win32/Release/PhysX3Extensions.lib)
target_link_libraries(Engine optimized ${PHYSX_LIB_DIR}/vc14-win32/Release/PxFoundation_x86.lib)

# Copy DLLs     TODO: Make a function to simplify this
if(${CMAKE_SYSTEM_NAME} MATCHES "Windows")
    execute_process(COMMAND cmake -E copy ${PHYSX_LIB_DIR}/vc14-win32/Debug/PhysX3DEBUG_x86.dll ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/Debug/PhysX3DEBUG_x86.dll)
	execute_process(COMMAND cmake -E copy ${PHYSX_LIB_DIR}/vc14-win32/Debug/PxFoundationDEBUG_x86.dll ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/Debug/PxFoundationDEBUG_x86.dll)
	execute_process(COMMAND cmake -E copy ${PHYSX_LIB_DIR}/vc14-win32/Debug/PhysX3CommonDEBUG_x86.dll ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/Debug/PhysX3CommonDEBUG_x86.dll)
	execute_process(COMMAND cmake -E copy ${PHYSX_LIB_DIR}/vc14-win32/Release/PhysX3_x86.dll ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/Release/PhysX3_x86.dll)
	execute_process(COMMAND cmake -E copy ${PHYSX_LIB_DIR}/vc14-win32/Release/PxFoundation_x86.dll ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/Release/PxFoundation_x86.dll)
	execute_process(COMMAND cmake -E copy ${PHYSX_LIB_DIR}/vc14-win32/Release/PhysX3Common_x86.dll ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/Release/PhysX3Common_x86.dll)
endif()
