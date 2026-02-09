#version 330 core
out vec4 FragColor;

in vec3 Normal;
in vec3 FragPos;

struct Material
{
	vec3 ambient;
	vec3 diffuse;
	vec3 specular;
	float shininess; // 反光度
};

struct Light
{
	vec3 Position;

	vec3 ambient;
	vec3 diffuse;
	vec3 specular;
	// 一个光源对它的ambient、diffuse和specular光照分量有着不同的强度
};

uniform Light light;
uniform Material material;


uniform vec3 lightPos;
uniform vec3 viewPos;

void main()
{
	// 环境光照
	vec3 ambient = material.ambient * light.ambient;

	// 漫反射光照
	vec3 norm = normalize(Normal);
	vec3 lightDir = normalize(lightPos - FragPos); // 计算光照方向向量并标准化
	float diff = max(dot(norm, lightDir), 0.0); 
	vec3 diffuse = (diff * material.diffuse) * light.diffuse; // 影响值乘光照即漫反射分量

	// 镜面光照
	vec3 viewDir = normalize(viewPos - FragPos); // 视线方向向量
	vec3 reflectDir = reflect(-lightDir, norm);  // 沿着法线的反射向量
	float spec = pow(max(dot(viewDir,  reflectDir), 0.0), material.shininess);
	vec3 specular = (material.specular * spec) * light.specular; // 镜面反射分量

	vec3 result = ambient + diffuse + specular;
	FragColor = vec4(result, 1.0);
}