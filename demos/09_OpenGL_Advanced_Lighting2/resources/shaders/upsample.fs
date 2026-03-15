#version 330 core
in vec2 TexCoords;
out vec4 FragColor;
uniform sampler2D downTex;
uniform sampler2D preTex;
uniform int currentLevel;
uniform float baseWeight;
uniform float growthRate;

float GetWeight(int level)
{
    return baseWeight * pow(1.0 + growthRate, float(level));
}

void main()
{
    vec4 down = texture(downTex, TexCoords);
    vec4 pre  = texture(preTex, TexCoords);
    FragColor = clamp(down * GetWeight(currentLevel) + pre, 0.0, 1.0);  // ¡û ¼Ó clamp
}