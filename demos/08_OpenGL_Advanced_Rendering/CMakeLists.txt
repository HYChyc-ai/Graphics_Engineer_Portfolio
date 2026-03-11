add_executable(OpenGL_Advanced_Rendering
    src/main.cpp
    resources/shaders/shader.fs
    resources/shaders/shader.vs
    resources/shaders/screenShader.fs
    resources/shaders/screenShader.vs
    resources/shaders/skybox.vs
    resources/shaders/skybox.fs
)

target_include_directories(OpenGL_Advanced_Rendering PRIVATE
    include
    ${CMAKE_SOURCE_DIR}/third_party/glad/include
    ${CMAKE_SOURCE_DIR}/third_party/glfw/include
    ${CMAKE_SOURCE_DIR}/third_party/stb/include
)

target_link_libraries(OpenGL_Advanced_Rendering PRIVATE
    glfw
    glad
    opengl32
)


set_target_properties(OpenGL_Advanced_Rendering PROPERTIES
    VS_DEBUGGER_WORKING_DIRECTORY
    ${CMAKE_CURRENT_SOURCE_DIR}
)
