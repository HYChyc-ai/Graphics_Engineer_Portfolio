#version 330 core
out vec4 FragColor;

in VS_OUT {
    vec3 FragPos;
    vec2 TexCoords;
    mat3 TBN;
} fs_in;

struct Material
{
    sampler2D albedo;
    sampler2D normalMap;
    sampler2D specularMap;
};

struct Light
{
    vec3 position;
    vec3 color;
    float constant;
    float linear;
    float quadratic;
};

uniform Material material;
uniform Light light;
uniform Light light2;

uniform samplerCube shadowMap;
uniform float far_plane;
uniform vec3 viewPos;

uniform samplerCube envMap; // 动态cubemap
uniform bool useEmissive;

const float PI = 3.14159265359;

float ShadowCalculation(vec3 fragPos);

// 法线分布函数 GGX
float DistributionGGX(vec3 N, vec3 H, float roughness)
{
    float a = roughness * roughness;
    float a2 = a * a;
    float NdotH = max(dot(N, H), 0.0);
    float denom = (NdotH * NdotH * (a2 - 1.0) + 1.0);
    return a2 / (PI * denom * denom);
}

// 几何遮蔽函数
float GeometrySchlickGGX(float NdotV, float roughness)
{
    float r = roughness + 1.0;
    float k = (r * r) / 8.0;
    return NdotV / (NdotV * (1.0 - k) + k);
}

float GeometrySmith(vec3 N, vec3 V, vec3 L, float roughness)
{
    float NdotV = max(dot(N, V), 0.0);
    float NdotL = max(dot(N, L), 0.0);
    return GeometrySchlickGGX(NdotV, roughness) * GeometrySchlickGGX(NdotL, roughness);
}

// 菲涅尔方程
vec3 FresnelSchlick(float cosTheta, vec3 F0)
{
    return F0 + (1.0 - F0) * pow(clamp(1.0 - cosTheta, 0.0, 1.0), 5.0);
}

// 单个光源的 PBR 计算
vec3 CalcPointLight(Light l, vec3 N, vec3 V, vec3 fragPos, vec3 albedo, float roughness, float metallic, vec3 F0)
{
    vec3 L = normalize(l.position - fragPos);
    vec3 H = normalize(V + L);

    float distance = length(l.position - fragPos);
    float attenuation = 1.0 / (l.constant + l.linear * distance + l.quadratic * distance * distance);
    vec3 radiance = l.color * attenuation;

    float NDF = DistributionGGX(N, H, roughness);
    float G = GeometrySmith(N, V, L, roughness);
    vec3 F = FresnelSchlick(max(dot(H, V), 0.0), F0);

    vec3 numerator = NDF * G * F;
    float denominator = 4.0 * max(dot(N, V), 0.0) * max(dot(N, L), 0.0) + 0.0001;
    vec3 specular = numerator / denominator;

    vec3 kS = F;
    vec3 kD = (vec3(1.0) - kS) * (1.0 - metallic);

    float NdotL = max(dot(N, L), 0.0);
    return (kD * albedo / PI + specular) * radiance * NdotL;
}

void main()
{
    vec3 albedo = pow(texture(material.albedo, fs_in.TexCoords).rgb, vec3(2.2)); // sRGB转线性

    vec3 normalSample = texture(material.normalMap, fs_in.TexCoords).rgb;
    vec3 N = normalize(normalSample * 2.0 - 1.0);
    N = normalize(fs_in.TBN * N);

    vec3 specularSample = texture(material.specularMap, fs_in.TexCoords).rgb;
    float roughness = max(1.0 - specularSample.r, 0.05); // 防止roughness为0
    float metallic = specularSample.g;

    vec3 V = normalize(viewPos - fs_in.FragPos);

    // 基础反射率，金属用albedo，非金属用0.04
    vec3 F0 = mix(vec3(0.04), albedo, metallic);

    float shadow = ShadowCalculation(fs_in.FragPos);
    

    // 三个光源累加
    vec3 Lo = vec3(0.0);
    Lo += CalcPointLight(light, N, V, fs_in.FragPos, albedo, roughness, metallic, F0) * (1.0 - shadow);
    Lo += CalcPointLight(light2, N, V, fs_in.FragPos, albedo, roughness, metallic, F0);
    
    // 环境光
    // 计算反射方向
    vec3 R = reflect(-V, N);

    // 从环境 Cubemap 采样反射颜色
    vec3 envSample = texture(envMap, R).rgb;

    // 菲涅尔
    vec3 F = FresnelSchlick(max(dot(N, V), 0.0), F0);
    vec3 kS = F;
    vec3 kD = (1.0 - kS) * (1.0 - metallic);

    // 漫反射环境光
    vec3 diffuseAmbient = kD * albedo * vec3(0.1, 0.08, 0.05);

    // 镜面反射：金属度越高反射越强
    vec3 specularAmbient = kS * envSample * (1.0 - roughness) * 0.3;


    vec3 ambient = diffuseAmbient + specularAmbient;

    // 自发光
    vec3 emissive = vec3(0.0);
    if(useEmissive)
    {
        float emissiveMask = specularSample.r > 0.66 ? specularSample.r : 0.0;
        emissive = albedo * emissiveMask * 10.0;
    }

    vec3 color = ambient + Lo + emissive;

    FragColor = vec4(color, 1.0);
}

float ShadowCalculation(vec3 fragPos)
{
    vec3 fragToLight = fragPos - light.position;
    float closestDepth = texture(shadowMap, fragToLight).r * far_plane;
    float currentDepth = length(fragToLight);

    float bias = 0.15;
    return currentDepth - bias > closestDepth ? 1.0 : 0.0;
    
}