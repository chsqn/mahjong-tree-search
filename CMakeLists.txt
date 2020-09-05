cmake_minimum_required(VERSION 2.8)

project(mahjong-tree-search)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_FLAGS_DEBUG "-O0 -Wall -g -DCHECK_HAND")
set(CMAKE_CXX_FLAGS_RELEASE "-O3 -Wall")

execute_process(COMMAND ${CMAKE_COMMAND} -E tar zxvf ${PROJECT_SOURCE_DIR}/index.tar.gz WORKING_DIRECTORY ${PROJECT_BINARY_DIR})

option(THREE_PLAYER "Three player mode" off)

if(THREE_PLAYER)
  add_definitions(-DTHREE_PLAYER)
endif()

include_directories(necessary-and-unnecessary-tiles-calculator)

add_library(calsht_dw STATIC necessary-and-unnecessary-tiles-calculator/calsht_dw.cpp)

add_executable(calc1-recu calc1.cpp)
target_link_libraries(calc1-recu calsht_dw)
target_compile_definitions(calc1-recu PRIVATE RECU)

add_executable(calc1-prob calc1.cpp)
target_link_libraries(calc1-prob calsht_dw)
target_compile_definitions(calc1-prob PRIVATE PROB)

add_executable(calc2-recu calc2.cpp)
target_link_libraries(calc2-recu calsht_dw)
target_compile_definitions(calc2-recu PRIVATE RECU)

add_executable(calc2-prob calc2.cpp)
target_link_libraries(calc2-prob calsht_dw)
target_compile_definitions(calc2-prob PRIVATE PROB)
