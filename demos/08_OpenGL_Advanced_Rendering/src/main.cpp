#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <STB_IMAGE/stb_image.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "SHADER.h"
#include "CAMERA.h"

#include <iostream>
#include <vector>
#include <map>


#define STB_IMAGE_IMPLEMENTATION
#include <STB_IMAGE/stb_image.h>

// Callback function
void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void mouse_callback(GLFWwindow* window, double xpos, double ypos);
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
void processInput(GLFWwindow* window);
unsigned int loadTexture(const char* path);
unsigned int loadCubemap(std::vector<std::string> faces);

// 窗口设置
const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;

// 摄像机
Camera camera(glm::vec3(0.0f, 0.0f, 3.0f));
float lastX = SCR_WIDTH / 2.0f;
float lastY = SCR_HEIGHT / 2.0f;
bool firstMouse = true;

// 跟踪时间差
float deltaTime = 0.0f;
float lastFrame = 0.0f;

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
	glfwSetCursorPosCallback(window, mouse_callback);
	glfwSetScrollCallback(window, scroll_callback);


	// 告诉GWLF隐藏光标，并捕捉它
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

	// glad: load all OpenGL function pointers
	//----------------------------------------
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout << "Fail to initialize GLAD" << std::endl;
		return -1;
	}

	// 调整openGL功能, 开启深度测试
	// 当它启用的时候，如果一个片段通过了深度测试的话，OpenGL会在深度缓冲中储存该片段的z值；
	// 如果没有通过深度缓冲，则会丢弃该片段
	glEnable(GL_DEPTH_TEST);
	// 深度测试函数，默认是GL_LESS比较运算符
	glDepthFunc(GL_LESS);

	// 启用混合
	glEnable(GL_BLEND);
	// 混合函数，设置源因子和目标因子
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	// 启用面剔除
	
	glEnable(GL_CULL_FACE);
	glCullFace(GL_BACK); // 默认值是back，剔除背面
	glFrontFace(GL_CCW); // 默认定义正面为逆时针，若想改为顺时针正面用GL_CW
	

	// 创建并编译着色器
	// ------------------------------------
	Shader shader("resources/shaders/shader.vs", "resources/shaders/shader.fs");
	Shader screenShader("resources/shaders/screenShader.vs", "resources/shaders/screenShader.fs");
	Shader skyboxShader("resources/shaders/skybox.vs", "resources/shaders/skybox.fs");
	// set up vertex data (and buffer(s)) and configure vertex attributes
	// ------------------------------------------------------------------
	float cubeVertices[] = {
		// positions          // normals      // texcoords
		-0.5f,-0.5f,-0.5f,  0.0f,0.0f,-1.0f,  0.0f,0.0f,
		 0.5f, 0.5f,-0.5f,  0.0f,0.0f,-1.0f,  1.0f,1.0f,
		 0.5f,-0.5f,-0.5f,  0.0f,0.0f,-1.0f,  1.0f,0.0f,
		 0.5f, 0.5f,-0.5f,  0.0f,0.0f,-1.0f,  1.0f,1.0f,
		-0.5f,-0.5f,-0.5f,  0.0f,0.0f,-1.0f,  0.0f,0.0f,
		-0.5f, 0.5f,-0.5f,  0.0f,0.0f,-1.0f,  0.0f,1.0f,

		-0.5f,-0.5f, 0.5f,  0.0f,0.0f, 1.0f,  0.0f,0.0f,
		 0.5f,-0.5f, 0.5f,  0.0f,0.0f, 1.0f,  1.0f,0.0f,
		 0.5f, 0.5f, 0.5f,  0.0f,0.0f, 1.0f,  1.0f,1.0f,
		 0.5f, 0.5f, 0.5f,  0.0f,0.0f, 1.0f,  1.0f,1.0f,
		-0.5f, 0.5f, 0.5f,  0.0f,0.0f, 1.0f,  0.0f,1.0f,
		-0.5f,-0.5f, 0.5f,  0.0f,0.0f, 1.0f,  0.0f,0.0f,

		-0.5f, 0.5f, 0.5f, -1.0f,0.0f,0.0f,  1.0f,0.0f,
		-0.5f, 0.5f,-0.5f, -1.0f,0.0f,0.0f,  1.0f,1.0f,
		-0.5f,-0.5f,-0.5f, -1.0f,0.0f,0.0f,  0.0f,1.0f,
		-0.5f,-0.5f,-0.5f, -1.0f,0.0f,0.0f,  0.0f,1.0f,
		-0.5f,-0.5f, 0.5f, -1.0f,0.0f,0.0f,  0.0f,0.0f,
		-0.5f, 0.5f, 0.5f, -1.0f,0.0f,0.0f,  1.0f,0.0f,

		 0.5f, 0.5f, 0.5f,  1.0f,0.0f,0.0f,  1.0f,0.0f,
		 0.5f,-0.5f,-0.5f,  1.0f,0.0f,0.0f,  0.0f,1.0f,
		 0.5f, 0.5f,-0.5f,  1.0f,0.0f,0.0f,  1.0f,1.0f,
		 0.5f,-0.5f,-0.5f,  1.0f,0.0f,0.0f,  0.0f,1.0f,
		 0.5f, 0.5f, 0.5f,  1.0f,0.0f,0.0f,  1.0f,0.0f,
		 0.5f,-0.5f, 0.5f,  1.0f,0.0f,0.0f,  0.0f,0.0f,

		-0.5f,-0.5f,-0.5f,  0.0f,-1.0f,0.0f,  0.0f,1.0f,
		 0.5f,-0.5f,-0.5f,  0.0f,-1.0f,0.0f,  1.0f,1.0f,
		 0.5f,-0.5f, 0.5f,  0.0f,-1.0f,0.0f,  1.0f,0.0f,
		 0.5f,-0.5f, 0.5f,  0.0f,-1.0f,0.0f,  1.0f,0.0f,
		-0.5f,-0.5f, 0.5f,  0.0f,-1.0f,0.0f,  0.0f,0.0f,
		-0.5f,-0.5f,-0.5f,  0.0f,-1.0f,0.0f,  0.0f,1.0f,

		-0.5f, 0.5f,-0.5f,  0.0f,1.0f,0.0f,  0.0f,1.0f,
		 0.5f, 0.5f, 0.5f,  0.0f,1.0f,0.0f,  1.0f,0.0f,
		 0.5f, 0.5f,-0.5f,  0.0f,1.0f,0.0f,  1.0f,1.0f,
		 0.5f, 0.5f, 0.5f,  0.0f,1.0f,0.0f,  1.0f,0.0f,
		-0.5f, 0.5f,-0.5f,  0.0f,1.0f,0.0f,  0.0f,1.0f,
		-0.5f, 0.5f, 0.5f,  0.0f,1.0f,0.0f,  0.0f,0.0f
	};

	float skyboxVertices[] = {
		// positions          
		-1.0f,  1.0f, -1.0f,
		-1.0f, -1.0f, -1.0f,
		 1.0f, -1.0f, -1.0f,
		 1.0f, -1.0f, -1.0f,
		 1.0f,  1.0f, -1.0f,
		-1.0f,  1.0f, -1.0f,

		-1.0f, -1.0f,  1.0f,
		-1.0f, -1.0f, -1.0f,
		-1.0f,  1.0f, -1.0f,
		-1.0f,  1.0f, -1.0f,
		-1.0f,  1.0f,  1.0f,
		-1.0f, -1.0f,  1.0f,

		 1.0f, -1.0f, -1.0f,
		 1.0f, -1.0f,  1.0f,
		 1.0f,  1.0f,  1.0f,
		 1.0f,  1.0f,  1.0f,
		 1.0f,  1.0f, -1.0f,
		 1.0f, -1.0f, -1.0f,

		-1.0f, -1.0f,  1.0f,
		-1.0f,  1.0f,  1.0f,
		 1.0f,  1.0f,  1.0f,
		 1.0f,  1.0f,  1.0f,
		 1.0f, -1.0f,  1.0f,
		-1.0f, -1.0f,  1.0f,

		-1.0f,  1.0f, -1.0f,
		 1.0f,  1.0f, -1.0f,
		 1.0f,  1.0f,  1.0f,
		 1.0f,  1.0f,  1.0f,
		-1.0f,  1.0f,  1.0f,
		-1.0f,  1.0f, -1.0f,

		-1.0f, -1.0f, -1.0f,
		-1.0f, -1.0f,  1.0f,
		 1.0f, -1.0f, -1.0f,
		 1.0f, -1.0f, -1.0f,
		-1.0f, -1.0f,  1.0f,
		 1.0f, -1.0f,  1.0f
	};

	float quadVertices[] = {
		// positions   // texCoords
		-1.0f,  1.0f,  0.0f, 1.0f,
		-1.0f, -1.0f,  0.0f, 0.0f,
		 1.0f, -1.0f,  1.0f, 0.0f,

		-1.0f,  1.0f,  0.0f, 1.0f,
		 1.0f, -1.0f,  1.0f, 0.0f,
		 1.0f,  1.0f,  1.0f, 1.0f
	};


	// cube VAO
	unsigned int cubeVAO, cubeVBO;
	glGenVertexArrays(1, &cubeVAO);
	glGenBuffers(1, &cubeVBO);
	glBindVertexArray(cubeVAO);
	glBindBuffer(GL_ARRAY_BUFFER, cubeVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(cubeVertices), &cubeVertices, GL_STATIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(2);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));

	// screen quad VAO
	unsigned int quadVAO, quadVBO;
	glGenVertexArrays(1, &quadVAO);
	glGenBuffers(1, &quadVBO);
	glBindVertexArray(quadVAO);
	glBindBuffer(GL_ARRAY_BUFFER, quadVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(quadVertices), &quadVertices, GL_STATIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)(2 * sizeof(float)));
	glBindVertexArray(0);

	// skybox VAO
	unsigned int skyboxVAO, skyboxVBO;
	glGenVertexArrays(1, &skyboxVAO);
	glGenBuffers(1, &skyboxVBO);
	glBindVertexArray(skyboxVAO);
	glBindBuffer(GL_ARRAY_BUFFER, skyboxVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(skyboxVertices), &skyboxVertices, GL_STATIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);


	// 加载纹理
	unsigned int cubeTexture = loadTexture("resources/textures/blue_ice.png");
	
	std::vector<std::string> faces
	{
		"resources/textures/skybox/right.jpg",
		"resources/textures/skybox/left.jpg",
		"resources/textures/skybox/top.jpg",
		"resources/textures/skybox/bottom.jpg",
		"resources/textures/skybox/front.jpg",
		"resources/textures/skybox/back.jpg"
	};
	unsigned int cubemapTexture = loadCubemap(faces);

	shader.use();
	shader.setInt("skybox", 0);
	shader.setInt("cubeTexture", 1);

	screenShader.use();
	screenShader.setInt("screenTexture", 0);

	skyboxShader.use();
	skyboxShader.setInt("skybox", 0);

	// 帧缓冲
	unsigned int framebuffer;
	glGenFramebuffers(1, &framebuffer);
	glBindFramebuffer(GL_FRAMEBUFFER, framebuffer);
	// 创建纹理附件（采样）
	unsigned int texture;
	glGenTextures(1, &texture);
	glBindTexture(GL_TEXTURE_2D, texture);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, 800, 600, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, texture, 0); // 附加到帧缓冲
	// 创建渲染缓冲对象附件（只进行深度测试）
	unsigned int rbo;
	glGenRenderbuffers(1, &rbo);
	glBindRenderbuffer(GL_RENDERBUFFER, rbo);
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, 800, 600);// 创建一个深度和模版渲染缓冲对象
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, rbo);
	// 检测是否成功
	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
		std::cout << "ERROR::FRAMEBUFFER:: Framebuffer is not complete!" << std::endl;
	glBindFramebuffer(GL_FRAMEBUFFER, 0); // 记得解绑

	// 渲染循环
	//------------
	while (!glfwWindowShouldClose(window))
	{
		// 在每一帧中计算出新的deltaTime以备后用
		float currentFrame = static_cast<float>(glfwGetTime());
		deltaTime = currentFrame - lastFrame;
		lastFrame = currentFrame;

		// 输入
		//------
		processInput(window);


		// 帧缓冲第一阶段
		glBindFramebuffer(GL_FRAMEBUFFER, framebuffer); // 切换默认缓冲为fbo
		glEnable(GL_DEPTH_TEST); // 同样需要开启深度测试
		glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); // 这两行是清理fbo中的颜色缓冲和深度缓冲
		
		// draw objects
		shader.use();
		glm::mat4 projection = glm::perspective(glm::radians(camera.Zoom), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);
		glm::mat4 view = camera.GetViewMatrix();
		glm::mat4 model = glm::mat4(1.0f);
		shader.setMat4("model", model);
		shader.setMat4("view", view);
		shader.setMat4("projection", projection);
		shader.setVec3("cameraPos", camera.Position);
		
		
		// cubes
		glBindVertexArray(cubeVAO);
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_CUBE_MAP, cubemapTexture);
		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_2D, cubeTexture);
		model = glm::translate(model, glm::vec3(-1.0f, 0.0f, -1.0f));
		shader.setMat4("model", model);
		glDrawArrays(GL_TRIANGLES, 0, 36);
		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(2.0f, 0.0f, 0.0f));
		shader.setMat4("model", model);
		glDrawArrays(GL_TRIANGLES, 0, 36);
		glBindVertexArray(0);
		

		// 最后画天空盒
		glDepthFunc(GL_LEQUAL);
		glDepthMask(GL_FALSE);

		skyboxShader.use();
		view = glm::mat4(glm::mat3(camera.GetViewMatrix()));
		skyboxShader.setMat4("view", view);
		skyboxShader.setMat4("projection", projection);
		glBindVertexArray(skyboxVAO);
		glBindTexture(GL_TEXTURE_CUBE_MAP, cubemapTexture);
		glDrawArrays(GL_TRIANGLES, 0, 36);

		glDepthMask(GL_TRUE);
		glDepthFunc(GL_LESS);
		

		// 帧缓冲第二阶段
		glBindFramebuffer(GL_FRAMEBUFFER, 0); // 切回默认缓冲，现在开始绘制出现在窗口
		glDisable(GL_DEPTH_TEST);
		glClearColor(1.0f, 1.0f, 1.0f, 1.0f); // set clear color to white (not really necessary actually, since we won't be able to see behind the quad anyways)
		glClear(GL_COLOR_BUFFER_BIT);

		screenShader.use(); // 用屏幕着色器绘制纹理
		glBindVertexArray(quadVAO);
		glBindTexture(GL_TEXTURE_2D, texture); // 绑定fbo里的纹理
		glDrawArrays(GL_TRIANGLES, 0, 6);


		glfwSwapBuffers(window);//交换缓冲区
		glfwPollEvents();//处理系统事件
	}


	// optional: de-allocate all resources once they've outlived their purpose:
	// ------------------------------------------------------------------------
	glDeleteVertexArrays(1, &cubeVAO);
	glDeleteVertexArrays(1, &quadVAO);
	glDeleteVertexArrays(1, &skyboxVAO);
	glDeleteBuffers(1, &cubeVBO);
	glDeleteBuffers(1, &quadVBO);
	glDeleteBuffers(1, &skyboxVBO);
	glDeleteRenderbuffers(1, &rbo);
	glDeleteFramebuffers(1, &framebuffer);

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

// 处理输入
// ---------------------------------------------------------------------------------------------------------
void processInput(GLFWwindow* window)
{
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(window, true);

	// 键盘输入处理
	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
		camera.ProcessKeyboard(FORWARD, deltaTime);
	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
		camera.ProcessKeyboard(BACKWARD, deltaTime);
	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
		camera.ProcessKeyboard(LEFT, deltaTime);
	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
		camera.ProcessKeyboard(RIGHT, deltaTime);
}

void mouse_callback(GLFWwindow* window, double xposIn, double yposIn)
{
	float xpos = static_cast<float>(xposIn);
	float ypos = static_cast<float>(yposIn);

	if (firstMouse)
	{
		lastX = xpos;
		lastY = ypos;
		firstMouse = false;
	}

	float xoffset = xpos - lastX;
	float yoffset = lastY - ypos; // reversed since y-coordinates go from bottom to top

	lastX = xpos;
	lastY = ypos;

	camera.ProcessMouseMovement(xoffset, yoffset);
}

void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
	camera.ProcessMouseScroll(static_cast<float>(yoffset));
}

// 从文件中加载2D纹理的函数
unsigned int loadTexture(char const* path)
{
	unsigned int textureID;
	glGenTextures(1, &textureID);

	int width, height, nrComponents;
	unsigned char* data = stbi_load(path, &width, &height, &nrComponents, 0);
	if (data)
	{
		GLenum format;
		if (nrComponents == 1)
			format = GL_RED;
		else if (nrComponents == 3)
			format = GL_RGB;
		else if (nrComponents == 4)
			format = GL_RGBA;

		glBindTexture(GL_TEXTURE_2D, textureID);
		glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

		stbi_image_free(data);
	}
	else
	{
		std::cout << "Texture failed to load at path: " << path << std::endl;
		stbi_image_free(data);
	}

	return textureID;
}

// 加载天空盒函数
unsigned int loadCubemap(std::vector<std::string> faces)
{
	unsigned int textureID;
	glGenTextures(1, &textureID);
	glBindTexture(GL_TEXTURE_CUBE_MAP, textureID);

	int width, height, nrChannels;
	for (unsigned int i = 0; i < faces.size(); i++)
	{
		unsigned char* data = stbi_load(faces[i].c_str(), &width, &height, &nrChannels, 0);
		if (data)
		{
			glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i,
				0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data
			);
			stbi_image_free(data);
		}
		else
		{
			std::cout << "Cubemap texture failed to load at path: " << faces[i] << std::endl;
			stbi_image_free(data);
		}
	}
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

	return textureID;
}