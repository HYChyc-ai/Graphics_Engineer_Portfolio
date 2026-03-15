#version 330 core
in vec2 TexCoords;
out vec4 FragColor;

uniform sampler2D sceneColorBuffer;
uniform sampler2D bloomColorBuffer;
uniform float exposure;
uniform float bloomStrength;

void main()
{
	vec3 hdrColor = texture(sceneColorBuffer, TexCoords).rgb;
    vec3 bloomColor = texture(bloomColorBuffer, TexCoords).rgb;

    // 叠加 Bloom（在 Tone Mapping 之前叠加，效果更自然）
    hdrColor += bloomColor * bloomStrength;

    // 曝光 Tone Mapping（和你原来的 screenShader 一致）
    vec3 mapped = vec3(1.0) - exp(-hdrColor * exposure);

    FragColor = vec4(mapped, 1.0);

}
