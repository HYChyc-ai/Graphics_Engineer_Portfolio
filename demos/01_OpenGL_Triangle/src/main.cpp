#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "SHADER.h"

#include <iostream>

#define STB_IMAGE_IMPLEMENTATION
#include <STB_IMAGE/stb_image.h>


// Callback function
void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow* window);

// settings
const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;


int main()
{
	// glfw: initialize and configure
	//-------------------------------
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3); 
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	// glfw window creation
	//---------------------
	GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "LeranOpenGL", NULL, NULL);
	if (window == NULL)
	{
		std::cout << "Fail to creat GLFW window" << std::endl;
		glfwTerminate();
		return -1;
	}
	glfwMakeContextCurrent(window);
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

	// glad: load all OpenGL function pointers
	//----------------------------------------
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout << "Fail to initialize GLAD" << std::endl;
		return -1;
	}


    // build and compile our shader program
    // ------------------------------------
	Shader ourShader("C:/Dev/CppSeries/learning_OpenGL/01_OpenGL_Triangle/resources/shaders/Shader.vs", "C:/Dev/CppSeries/learning_OpenGL/01_OpenGL_Triangle/resources/shaders/Shader.fs");

    // set up vertex data (and buffer(s)) and configure vertex attributes
    // ------------------------------------------------------------------
    float vertices[] = {
         0.5f, -0.5f, 0.0f, 1.0f, 0.0f, 0.0f,
        -0.5f, -0.5f, 0.0f, 0.0f, 1.0f, 0.0f,
         0.0f,  0.5f, 0.0f, 0.0f, 0.0f, 1.0f
    };

    unsigned int VAO, VBO;

    glGenVertexArrays(1, &VAO);
    glBindVertexArray(VAO);
    
    glGenBuffers(1, &VBO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);//传输

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6* sizeof(float), (void*)0);//说明接线规则
    glEnableVertexAttribArray(0);//允许 GPU 在绘制时，从 VAO 里按照规则读取 location = 0 的数据。

	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);



	// render loop
	//------------
	while (!glfwWindowShouldClose(window))
	{
		// input
		//------
		processInput(window);

		// render
		//------------------
		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);//背景色
		glClear(GL_COLOR_BUFFER_BIT);//清屏

        
		ourShader.use();
		glBindVertexArray(VAO);
		glDrawArrays(GL_TRIANGLES, 0, 3);
		
		glfwSwapBuffers(window);//交换缓冲区
		glfwPollEvents();//处理系统事件
	}


    // optional: de-allocate all resources once they've outlived their purpose:
    // ------------------------------------------------------------------------
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
   
	// glfw: terminate, clearing all previously allocated GLFW resources
	//------------------------------------------------------------------
	glfwTerminate();
	return 0;
}


// glfw: whenever the window size changed (by OS or user resize) this callback function executes
// ---------------------------------------------------------------------------------------------
void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
	glViewport(0, 0, width, height);
}

// process all input: query GLFW whether relevant keys are pressed/released this frame and react accordingly
// ---------------------------------------------------------------------------------------------------------
void processInput(GLFWwindow* window)
{
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(window, true);
}