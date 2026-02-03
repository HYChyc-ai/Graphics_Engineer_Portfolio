add_executable(OpenGL_Triangle
    src/main.cpp
)

target_include_directories(OpenGL_Triangle PRIVATE
    include
    ${PROJECT_SOURCE_DIR}/third_party/stb/include
)

target_link_libraries(OpenGL_Triangle PRIVATE
    glfw
    glad
    opengl32
)

# VS 下把它设为启动项目
set_property(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
    PROPERTY VS_STARTUP_PROJECT OpenGL_Triangle)
