#version 330 core
out vec4 FragColor;

in vec3 Normal;
in vec3 Position;
in vec2 TexCoord;

uniform vec3 cameraPos;
uniform samplerCube skybox;
uniform sampler2D cubeTexture;

void main()
{   
    // ≤ƒ÷ 
    vec3 albedo = texture(cubeTexture, TexCoord).rgb;
    // ∑¥…‰
    vec3 I = normalize(Position - cameraPos);
    vec3 R = reflect(I, normalize(Normal));
    vec3 reflection = texture(skybox, R).rgb;

    // ªÏ∫œ
    float reflectStrength = 0.4;

    vec3 color = mix(albedo, reflection, reflectStrength);

    FragColor = vec4(color, 1.0);
}