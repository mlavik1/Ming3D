project(NativeUI)

set(SourceDir ${PROJECT_SOURCE_DIR}/Source)

# Gather c++ files
file(GLOB_RECURSE SRC_FILES 
	Source/*.cpp
	Source/*.h
)

include_directories(${SourceDir})

add_library(NativeUI STATIC ${SRC_FILES})

install(TARGETS NativeUI DESTINATION /lib)
