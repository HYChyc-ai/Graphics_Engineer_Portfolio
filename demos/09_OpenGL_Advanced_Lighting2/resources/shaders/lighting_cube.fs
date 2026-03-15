#version 330 core
out vec4 FragColor;
void main()
{
    FragColor = vec4(3.0, 3.0, 3.0, 1.0);  // 超过阈值才能产生光晕
}