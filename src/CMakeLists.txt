cmake_minimum_required(VERSION 3.10)
project(OpenGLPhysicsEngine)

set(CMAKE_CXX_STANDARD 11)

include_directories(${CMAKE_SOURCE_DIR}/common)
include_directories(${CMAKE_SOURCE_DIR}/external)

file(GLOB_RECURSE SOURCES
    main.cpp
    lib/*.cpp
    ../common/*.cpp
)

add_executable(OpenGLPhysicsEngine ${SOURCES})

# Link any necessary libraries here
# target_link_libraries(OpenGLPhysicsEngine <libraries>)