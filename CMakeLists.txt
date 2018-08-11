cmake_minimum_required(VERSION 3.1)
set(ProjectName Edison)

include(GeneConfig.cmake)

project(${ProjectName})

set(GeneLibDir ${GeneEngineDirectory})
set(CMAKE_MODULE_PATH ${GeneLibDir}/Setup/CMake)
add_subdirectory(${GeneLibDir} LibraryBuild)
include(GeneBuild)

include_directories(${GeneLibDir}/Gene/Engine/Public)
include_directories(${GeneLibDir}/Dependencies/FreeType/include)

# Set your source files here
set(Source
	Source/Main.cc
	Source/Utils.cc
	Source/Global.cc
	Source/Player.cc

	Source/Utils.h
	Source/Camera.h
	Source/Global.h
	Source/Config.h
	Source/Player.h
	Source/Spritesheet.h
	Source/TileMap.h
	Source/Level.h
)

gene_add_target(${ProjectName} "${Source}")
gene_setup_engine_deps(${ProjectName})

add_definitions(-DASSETS_DIRECTORY="${CMAKE_CURRENT_SOURCE_DIR}/Data/" -DASSETS_OUT_DIRECTORY="${CMAKE_CURRENT_BINARY_DIR}/Data/")

add_custom_command(TARGET ${ProjectName} POST_BUILD 
	COMMAND ${CMAKE_COMMAND} -E 
		copy_directory 
			"${CMAKE_CURRENT_SOURCE_DIR}/Data/" 
			"${CMAKE_CURRENT_BINARY_DIR}/Data/" 
	COMMENT "Copying assets"
)
