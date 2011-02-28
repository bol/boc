cmake_minimum_required(VERSION 2.6)

project(otop)

set(CMAKE_C_FLAGS "-ggdb -Wall")

set(SOURCE_FILES otop
				mem 
				print
				update)

add_executable(otop ${SOURCE_FILES}) 
