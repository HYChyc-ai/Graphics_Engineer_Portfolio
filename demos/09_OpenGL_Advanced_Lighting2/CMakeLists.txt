add_executable(OpenGL_Advanced_Lighting2
    src/main.cpp
    resources/shaders/cube_shader.fs
    resources/shaders/cube_shader.vs
    resources/shaders/screenShader.fs
    resources/shaders/screenShader.vs
    resources/shaders/lighting_cube.vs
    resources/shaders/lighting_cube.fs
    resources/shaders/shadow_mapping_depth.vs
    resources/shaders/shadow_mapping_depth.fs
)

target_include_directories(OpenGL_Advanced_Lighting2 PRIVATE
    include
    ${CMAKE_SOURCE_DIR}/third_party/glad/include
    ${CMAKE_SOURCE_DIR}/third_party/glfw/include
    ${CMAKE_SOURCE_DIR}/third_party/stb/include
)

target_link_libraries(OpenGL_Advanced_Lighting2 PRIVATE
    glfw
    glad
    opengl32
)


set_target_properties(OpenGL_Advanced_Lighting2 PROPERTIES
    VS_DEBUGGER_WORKING_DIRECTORY
    ${CMAKE_CURRENT_SOURCE_DIR}
)
