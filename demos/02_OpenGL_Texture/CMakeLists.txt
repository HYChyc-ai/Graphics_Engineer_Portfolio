add_executable(OpenGL_Texture
    src/main.cpp
)

target_include_directories(OpenGL_Texture PRIVATE
    include
    ${CMAKE_SOURCE_DIR}/third_party/glad/include
    ${CMAKE_SOURCE_DIR}/third_party/glfw/include
    ${CMAKE_SOURCE_DIR}/third_party/stb/include
)

target_link_libraries(OpenGL_Texture PRIVATE
    glfw
    glad
    opengl32
)


set_target_properties(OpenGL_Texture PROPERTIES
    VS_DEBUGGER_WORKING_DIRECTORY
    ${CMAKE_CURRENT_SOURCE_DIR}
)
