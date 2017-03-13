cmake_minimum_required (VERSION 2.8.12)
project (tsuki-studio)

set (tsukistudio_VERSION_MAJOR 1)
set (tsukistudio_VERSION_MINOR 0)
set (obsstudio_VERSION 18.0.1)

set (OBSSourcePath $ENV{OBSSourcePath})

option (BUILD_X64
	"Build for x64 architecture" ON)

if (BUILD_X64)
	set (BUILD_PLATFORM x64)
	set (OBS_BUILD_DIR build64)
endif ()

if (NOT BUILD_PLATFORM)
	message(FATAL_ERROR "No platform specified")
endif ()
	
# Source
file (GLOB SOURCES ${CMAKE_SOURCE_DIR}/src/*.cpp)
file (GLOB HEADER_FILES ${CMAKE_SOURCE_DIR}/include/*.h)

include_directories (include)
add_library (${PROJECT_NAME} SHARED
	${SOURCES}
	${HEADER_FILES}
)

# libobs
include_directories(${OBSSourcePath})
add_library (libobs SHARED IMPORTED)
set_property (TARGET libobs PROPERTY IMPORTED_LOCATION ${OBSSourcePath}/../${OBS_BUILD_DIR}/libobs/Release/obs.dll)
set_property (TARGET libobs PROPERTY IMPORTED_IMPLIB ${OBSSourcePath}/../${OBS_BUILD_DIR}/libobs/Release/obs.lib)
target_link_libraries (${PROJECT_NAME} libobs)

# lua
include_directories (${CMAKE_SOURCE_DIR}/deps/${BUILD_PLATFORM}/lua53/include)
add_library (lua53 STATIC IMPORTED)
set_property (TARGET lua53 PROPERTY IMPORTED_LOCATION ${CMAKE_SOURCE_DIR}/deps/${BUILD_PLATFORM}/lua53/liblua53.a)
target_link_libraries (${PROJECT_NAME} lua53)
