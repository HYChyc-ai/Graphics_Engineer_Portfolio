#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal; // 法向量
layout (location = 2) in vec2 aTexCoords;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;
uniform mat3 normalMatrix;

out vec3 Normal;  // 输出法向量
out vec3 FragPos; // 输出顶点在世界空间中的坐标（乘上model）
out vec2 TexCoords;

void main()
{
	Normal = normalize(normalMatrix * aNormal);
	FragPos = vec3(model * vec4(aPos, 1.0));
	gl_Position = projection * view * vec4(FragPos, 1.0);
	TexCoords = aTexCoords;
}

// 矩阵求逆是一项对于着色器开销很大的运算，因为它必须在场景中的每一个顶点上进行，所以应该尽可能地避免在着色器中进行求逆运算。
// 以学习为目的的话这样做还好，但是对于一个高效的应用来说，
// 最好先在CPU上计算出法线矩阵，再通过uniform把它传递给着色器（就像模型矩阵一样）。