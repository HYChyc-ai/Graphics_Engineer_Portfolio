#version 330 core
in vec2 TexCoords;
out vec4 FragColor;
uniform sampler2D colorBuffer;
uniform int isy;

void main()
{
    vec2 texSize = vec2(textureSize(colorBuffer, 0));
    vec2 offset = vec2(1.0) / texSize;
    float kernel[5] = float[](0.0545, 0.2447, 0.4026, 0.2447, 0.0545);
    vec3 col = vec3(0.0);
    for(int i = -2; i <= 2; i++)
    {
        vec2 texOffset = isy == 1 ? vec2(0, i * offset.y) : vec2(i * offset.x, 0);
        col += kernel[i+2] * texture(colorBuffer, TexCoords + texOffset).rgb;
    }
    FragColor = vec4(col, 1.0);
}