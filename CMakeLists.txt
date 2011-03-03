cmake_minimum_required(VERSION 2.6)

project(boc)

set(CMAKE_C_FLAGS "-ggdb -Wall -Wextra")

set(SOURCE_FILES boc
		mem 
		print
		update)

add_executable(boc ${SOURCE_FILES}) 
