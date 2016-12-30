cmake_minimum_required(VERSION 3.0)
project(server)

set(PROJECT_ROOT_DIR ${CMAKE_CURRENT_SOURCE_DIR})
set(PROJECT_SOURCE_DIR ${PROJECT_ROOT_DIR}/src)
set(PROJECT_DEP_DIR ${PROJECT_ROOT_DIR}/deps)
set(PROJECT_SOURCES 
  ${PROJECT_SOURCE_DIR}/main.c
  )

add_executable(server ${PROJECT_SOURCES})


