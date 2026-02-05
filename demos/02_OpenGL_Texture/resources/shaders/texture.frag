#version 330 core
out vec4 FragColor;

in vec3 ourColor;
in vec2 TexCoord;

uniform sampler2D texture1;  // sampler2D = 纹理单元编号
uniform sampler2D texture2;

void main()
{
	FragColor = mix(texture(texture1, TexCoord), texture(texture2, TexCoord), 0.2);
	// 用texture函数来采样纹理颜色
	// mix函数接收两个值作为参数，并根据第三个值进行线性插值
}