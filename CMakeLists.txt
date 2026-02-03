cmake_minimum_required(VERSION 3.20)
project(OpenGL_Demos)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# 第三方库
add_subdirectory(third_party/glfw)

# glad（手动建库）
add_library(glad
    third_party/glad/src/glad.c
)
target_include_directories(glad PUBLIC
    third_party/glad/include
)

# demos
add_subdirectory(demos/01_OpenGL_Triangle)
