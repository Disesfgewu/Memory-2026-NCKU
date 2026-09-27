cmake_minimum_required(VERSION 3.10)
project(lab1_precompiled_compare LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

if(NOT CMAKE_SYSTEM_NAME STREQUAL "Linux" OR
   NOT CMAKE_SYSTEM_PROCESSOR MATCHES "^(x86_64|amd64)$")
  message(FATAL_ERROR "The bundled lab1.a was built for Linux x86_64")
endif()

set(LAB1_ARCHIVE "${CMAKE_CURRENT_SOURCE_DIR}/lab1.a")
if(NOT EXISTS "${LAB1_ARCHIVE}")
  message(FATAL_ERROR "Missing precompiled archive: ${LAB1_ARCHIVE}")
endif()

add_library(lab1 STATIC IMPORTED GLOBAL)
set_target_properties(lab1 PROPERTIES IMPORTED_LOCATION "${LAB1_ARCHIVE}")

set(LAB1_HEADER_ROOT "${CMAKE_CURRENT_SOURCE_DIR}/source")
add_executable(lab1_compare
  "${CMAKE_CURRENT_SOURCE_DIR}/lab1_read_write_student.cc")
target_include_directories(lab1_compare PRIVATE
  "${CMAKE_CURRENT_SOURCE_DIR}"
  "${LAB1_HEADER_ROOT}"
  "${LAB1_HEADER_ROOT}/ssd"
  "${LAB1_HEADER_ROOT}/lib/drampower/src"
  "${LAB1_HEADER_ROOT}/ssd/lib/mcpat"
  "${LAB1_HEADER_ROOT}/ssd/lib/mcpat/cacti"
  "${LAB1_HEADER_ROOT}/ssd/lib/inih")

set(THREADS_PREFER_PTHREAD_FLAG ON)
find_package(Threads REQUIRED)
target_link_libraries(lab1_compare PRIVATE lab1 Threads::Threads m)
target_compile_options(lab1_compare PRIVATE -O2 -Wall -Wextra -pthread)
