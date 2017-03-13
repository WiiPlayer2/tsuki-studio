cmake_minimum_required(VERSION 2.8.12)
project(tsuki-studio)

set (tsukistudio_VERSION_MAJOR 1)
set (tsukistudio_VERSION_MINOR 0)
set (obsstudio_VERSION 18.0.1)

set (OBSSourcePath $ENV{OBSSourcePath})

option(BUILD_X64
	"Build for x64 architecture" ON)

# Source
file (GLOB SOURCES src/*.cpp)
file (GLOB HEADER_FILES include/*.h)
set (HEADER_FILES ${HEADER_FILES})

include_directories(include ${OBSSourcePath})
add_library(${PROJECT_NAME} SHARED
	${SOURCES}
	${HEADER_FILES}
)

# libobs
add_library(libobs SHARED IMPORTED)
set_property(TARGET libobs PROPERTY IMPORTED_LOCATION ${OBSSourcePath}/../build64/libobs/Debug/obs.dll)
set_property(TARGET libobs PROPERTY IMPORTED_IMPLIB ${OBSSourcePath}/../build64/libobs/Debug/obs.lib)
target_link_libraries(${PROJECT_NAME} libobs)
