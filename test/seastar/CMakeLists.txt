cmake_minimum_required(VERSION 2.4) 

set(src_name "seastar-started")
add_executable(${src_name} "${src_name}.cc")
target_link_libraries(${src_name} ${link_libraries_name})
