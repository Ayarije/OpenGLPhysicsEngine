cmake_minimum_required(VERSION 3.10)
project(OpenGLPhysicsEngine)

set(CMAKE_CXX_STANDARD 11)

# 1. Initialize Dependencies
find_package(OpenGL REQUIRED)

# Build the external libraries (GLFW, GLEW, etc.) using their own CMakeLists
add_subdirectory(external)

# 2. Setup Include Directories
# We add ${CMAKE_SOURCE_DIR} so that #include "common/shader.hpp" works
include_directories(
    ${CMAKE_SOURCE_DIR}
    ${CMAKE_SOURCE_DIR}/external/glfw-3.1.2/include
    ${CMAKE_SOURCE_DIR}/external/glew-1.13.0/include
    ${CMAKE_SOURCE_DIR}/external/glm-0.9.7.1
    ${CMAKE_SOURCE_DIR}/external/AntTweakBar-1.16/include
)

# 3. Gather Source Files
# We must include both 'src' AND 'common' files
file(GLOB_RECURSE SOURCES
    "src/*.cpp"
    "common/*.cpp"
)

# 4. Create Executable
add_executable(OpenGLPhysicsEngine ${SOURCES})

# 5. Link Libraries
# These target names (glfw, GLEW_1130) are defined in your external/CMakeLists.txt
target_link_libraries(OpenGLPhysicsEngine
    ${OPENGL_LIBRARIES}
    glfw
    GLEW_1130
)