cmake_minimum_required(VERSION 2.4) 

message(STATUS "$ENV{THRIFT_HOME}/include")

include_directories(
  $ENV{THRIFT_HOME}/include 
  ${PROJECT_SOURCE_DIR}/test/thrift
)

link_libraries(
  $ENV{THRIFT_HOME}/lib
)

aux_source_directory(gen-cpp SRCS_GENCPP)

add_library(thrift_tutorial STATIC ${SRCS_GENCPP})
target_link_libraries(thrift_tutorial thrift)

#set(src_name "app_client")
#add_executable(${src_name} "${src_name}.cc")
#target_link_libraries(${src_name} thrift_tutorial thrift)

set(src_name "app_server")
add_executable(${src_name} "${src_name}.cc")
target_link_libraries(${src_name} thrift_tutorial thrift)
