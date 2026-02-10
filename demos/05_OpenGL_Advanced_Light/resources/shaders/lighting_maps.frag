#version 330 core
out vec4 FragColor;

in vec3 Normal;
in vec3 FragPos;

struct Material
{
	sampler2D diffuse; // 一张贴图解决所有颜色
	sampler2D specular;
	float shininess; // 反光度
};

in vec2 TexCoords;

struct PointLight
{
	vec3 position;

	float constant;
    float linear;
    float quadratic;

    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};

struct SpotLight
{
	vec3 position;
	vec3 direction;

	float cutOff;
	float outerCutOff;

	vec3 ambient;
	vec3 diffuse;
	vec3 specular;
	
	float constant;
	float linear;
	float quadratic;
};

uniform PointLight pointLight;
uniform SpotLight spotLight;
uniform Material material;


uniform vec3 viewPos;

vec3 CalcPointLight(PointLight light, vec3 normal, vec3 fragPos, vec3 viewDir);
vec3 CalcSpotLight(SpotLight light, vec3 normal, vec3 fragPos, vec3 viewDir);

void main()
{
	vec3 normal = normalize(Normal);
	vec3 viewDir = normalize(viewPos - FragPos);

	vec3 result = vec3(0.0);

	result += CalcPointLight(pointLight, normal, FragPos, viewDir);
	result += CalcSpotLight(spotLight,  normal, FragPos, viewDir);
	// 两个result，叠加两次光照贡献

	FragColor = vec4(result, 1.0);
}


// 点光源计算函数
vec3 CalcPointLight(PointLight light, vec3 normal, vec3 fragPos, vec3 viewDir)
{
	vec3 lightDir = normalize(light.position - fragPos);

	float diff = max(dot(normal, lightDir), 0.0);

	vec3 reflectDir = reflect(-lightDir, normal);
	float spec = pow(max(dot(viewDir,  reflectDir), 0.0), material.shininess);

	float dist = length(light.position - FragPos);
	float attenuation = 1.0 / (light.constant + light.linear * dist + light.quadratic * (dist * dist));

	vec3 ambient  = light.ambient * vec3(texture(material.diffuse, TexCoords)) * diff;
	vec3 diffuse  = vec3(texture(material.diffuse, TexCoords)) * diff * light.diffuse;
	vec3 specular = vec3(texture(material.specular, TexCoords)) * spec * light.specular;

	ambient  *= attenuation;
	diffuse  *= attenuation;
	specular *= attenuation;

	return ambient + diffuse + specular;
}

// 聚光灯计算函数
vec3 CalcSpotLight(SpotLight light, vec3 normal, vec3 fragPos, vec3 viewDir)
{
    vec3 lightDir = normalize(light.position - fragPos);

    float theta = dot(lightDir, normalize(-light.direction));
    float epsilon = light.cutOff - light.outerCutOff;
    float intensity = clamp(
        (theta - light.outerCutOff) / epsilon,
        0.0,
        1.0
    );
	// 使用了clamp函数，把第一个参数约束(Clamp)在了0.0到1.0之间。这保证强度值不会在[0, 1]区间之外。

    float diff = max(dot(normal, lightDir), 0.0);

    vec3 reflectDir = reflect(-lightDir, normal);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);

    float dist = length(light.position - fragPos);
	float attenuation = 1.0 / (light.constant + light.linear * dist + light.quadratic * (dist * dist));
    
    vec3 ambient  = light.ambient * vec3(texture(material.diffuse, TexCoords)) * diff;
    vec3 diffuse  = light.diffuse * diff * vec3(texture(material.diffuse, TexCoords));
    vec3 specular = vec3(texture(material.specular, TexCoords)) * spec * light.specular;

    ambient  *= attenuation * intensity;
    diffuse  *= attenuation * intensity;
    specular *= attenuation * intensity;

    return ambient + diffuse + specular;
}