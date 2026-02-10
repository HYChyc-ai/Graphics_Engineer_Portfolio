add_executable(OpenGL_Advanced_Light
    src/main.cpp
)

target_include_directories(OpenGL_Advanced_Light PRIVATE
    include
    ${CMAKE_SOURCE_DIR}/third_party/glad/include
    ${CMAKE_SOURCE_DIR}/third_party/glfw/include
    ${CMAKE_SOURCE_DIR}/third_party/stb/include
)

target_link_libraries(OpenGL_Advanced_Light PRIVATE
    glfw
    glad
    opengl32
)


set_target_properties(OpenGL_Advanced_Light PROPERTIES
    VS_DEBUGGER_WORKING_DIRECTORY
    ${CMAKE_CURRENT_SOURCE_DIR}
)
