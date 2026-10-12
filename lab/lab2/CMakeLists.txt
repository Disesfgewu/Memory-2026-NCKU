cmake_minimum_required(VERSION 3.10)
project(lab2_compare LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS ON)

find_package(Threads REQUIRED)

set(LAB2_INCLUDE_DIRECTORIES
  ${CMAKE_CURRENT_SOURCE_DIR}/include
  ${CMAKE_CURRENT_SOURCE_DIR}/include/lib/mcpat
  ${CMAKE_CURRENT_SOURCE_DIR}/include/lib/mcpat/cacti
  ${CMAKE_CURRENT_SOURCE_DIR}/include/lib/inih
  ${CMAKE_CURRENT_SOURCE_DIR}/include/drampower
)

add_library(lab2_student_gc OBJECT lab2_gc_student.cc)
target_include_directories(lab2_student_gc PRIVATE ${LAB2_INCLUDE_DIRECTORIES})
target_compile_options(lab2_student_gc PRIVATE -O2 -g -Wall -Wextra -Werror)

add_executable(lab2_reference link_anchor.cc)
add_executable(lab2_student
  link_anchor.cc
  $<TARGET_OBJECTS:lab2_student_gc>
)

foreach(target lab2_reference lab2_student)
  target_link_libraries(${target} PRIVATE
    "-Wl,--start-group"
    ${CMAKE_CURRENT_SOURCE_DIR}/lab2.a
    "-Wl,--end-group"
    Threads::Threads
  )
  set_target_properties(${target} PROPERTIES ENABLE_EXPORTS ON)
endforeach()
