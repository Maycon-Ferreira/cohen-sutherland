cmake_minimum_required(VERSION 3.10)
project(cohen_sutherland C)

set(CMAKE_C_STANDARD 11)

find_package(OpenGL REQUIRED)
find_package(GLUT REQUIRED)

include_directories(${OPENGL_INCLUDE_DIRS} ${GLUT_INCLUDE_DIRS})

add_executable(cohen_sutherland main.c)

target_link_libraries(cohen_sutherland ${OPENGL_LIBRARIES} ${GLUT_LIBRARY} )