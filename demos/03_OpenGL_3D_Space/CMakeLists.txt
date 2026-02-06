add_executable(OpenGL_3D_Space
    src/main.cpp
)

target_include_directories(OpenGL_3D_Space PRIVATE
    include
    ${CMAKE_SOURCE_DIR}/third_party/glad/include
    ${CMAKE_SOURCE_DIR}/third_party/glfw/include
    ${CMAKE_SOURCE_DIR}/third_party/stb/include
)

target_link_libraries(OpenGL_3D_Space PRIVATE
    glfw
    glad
    opengl32
)


set_target_properties(OpenGL_3D_Space PROPERTIES
    VS_DEBUGGER_WORKING_DIRECTORY
    ${CMAKE_CURRENT_SOURCE_DIR}
)
