cmake_minimum_required(VERSION 2.4)

project(openmit-ps)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -g -O3 -fPIC -Wall -mssse3")
set(CMAKE_CXX_FLAGS_DEBUG          "-O0 -g")
set(CMAKE_CXX_FLAGS_MINSIZEREL     "-Os -DNDEBUG")
set(CMAKE_CXX_FLAGS_RELEASE        "-O4 -DNDEBUG")
set(CMAKE_CXX_FLAGS_RELWITHDEBINFO "-O2 -g")

if ("${CMAKE_CXX_COMPILER_ID}" MATCHES "GNU")
	execute_process(COMMAND ${CMAKE_CXX_COMPILER} -dumpversion OUTPUT_VARIABLE GCC_VERSION)
	if (NOT (GCC_VERSION VERSION_GREATER 7.0 OR GCC_VERSION VERSION_EQUAL 7.0))
		message(FATAL_ERROR "${PROJECT_NAME} project requires g++ 7.0 or greater.")
  else ()
    #set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=gnu++17 ")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++17 ")
	endif()
elseif ("${CMAKE_CXX_COMPILER_ID}" MATCHES "Clang")
	set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -stdlib=libc++")
else ()
	message(FATAL_ERROR "Your C++ compiler does not support C++11.")
endif () 

# seastar check 
message(STATUS "seastar path: $ENV{SEASTAR}")

# include 
include_directories(
  $ENV{SEASTAR}
  $ENV{SEASTAR}/build/release/c-ares
  $ENV{SEASTAR}/build/release/gen
  $ENV{SEASTAR}/fmt
  )

# Library directory
link_directories(
  $ENV{SEASTAR}/build/release
  $ENV{SEASTAR}/build/release/fmt/fmt
  )

# Link_libraries name
set(seastar_libraries_name
  -lseastar
  -lcares-seastar
  -lfmt)

set(link_libraries_name 
  -lboost_program_options
  -lboost_system
  -lboost_filesystem
  -lboost_thread
  -lcryptopp
  -ldl
  -lgcc_s
  -lgnutls
  -lgnutlsxx
  -lhwloc
  -llz4
  -lm
  -lnuma
  -lpciaccess
  -lprotobuf
  -pthread
  -lrt
  -lstdc++
  -lunwind
  -lxml2
  -lyaml-cpp
  -lz
  ${seastar_libraries_name}
  )

# Output 
set(LIBRARY_OUTPUT_PATH ${PROJECT_SOURCE_DIR})
set(EXECUTABLE_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/bin) 

# Add subdirectory 
#add_subdirectory(test/seastar)
add_subdirectory(test/thrift)
