cmake_minimum_required(VERSION 3.0)
project(client)

set(PROJECT_ROOT_DIR ${CMAKE_CURRENT_SOURCE_DIR})
set(PROJECT_SOURCE_DIR ${PROJECT_ROOT_DIR}/src)
set(PROJECT_DEP_DIR ${PROJECT_ROOT_DIR}/deps)
set(PROJECT_SOURCES 
  ${PROJECT_SOURCE_DIR}/main.c
  )

add_executable(client ${PROJECT_SOURCES})

# If using MSVC (Windows C compiler), make sure to link in the winsock library.
if(MSVC)
  target_link_libraries(client ws2_32.lib)
endif(MSVC)
