cmake_minimum_required(VERSION 3.13.4)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

project(heightmap CXX)

find_package(glm REQUIRED)
find_package(glfw3 REQUIRED)
find_package(GLEW REQUIRED)
find_package(OpenGL REQUIRED)

add_executable(${PROJECT_NAME} src/main.cc src/shader.cc src/map.cc)

target_compile_options(${PROJECT_NAME} PRIVATE -Wall -Werror -O0 -g)
target_link_libraries(${PROJECT_NAME} OpenGL::GL OpenGL::GLU glfw glm GLEW::GLEW)
