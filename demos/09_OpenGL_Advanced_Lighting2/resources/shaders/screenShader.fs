#version 330 core
out vec4 FragColor;

in vec2 TexCoords;

uniform sampler2D screenTexture;



void main()
{
   FragColor = texture(screenTexture, TexCoords);
} 

// 反向。用1.0减去vec3颜色
// 灰度。取所有颜色分量相加，将他们平均化。（进一步的话使用加权的通道）
// 核效果。opengl实例是锐化核
// 模糊。改变核矩阵，注意除以16以防太亮
// 边缘检测。