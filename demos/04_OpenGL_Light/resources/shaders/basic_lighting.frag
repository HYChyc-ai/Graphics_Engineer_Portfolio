#version 330 core
out vec4 FragColor;

in vec3 Normal;
in vec3 FragPos;

uniform vec3 objectColor;
uniform vec3 lightColor;
uniform vec3 lightPos;
uniform vec3 viewPos;

vec3 norm = normalize(Normal);
vec3 lightDir = normalize(lightPos - FragPos); // 计算光照方向向量并标准化


void main()
{
	// 环境光照
	float ambientStrength = 0.1; // 很小的环境常量因子
	vec3 ambient = ambientStrength * lightColor; // 环境光分量

	// 漫反射光照
	float diff = max(dot(norm, lightDir), 0.0); 
	// 对norm，lighDir进行点乘得到漫反射影响值，当大于90度时分量为负，为避免负数用max函数取两参数间更大的
	vec3 diffuse = diff * lightColor; // 影响值乘光照即漫反射分量

	// 镜面光照
	float specularStrength = 0.5; // 镜面强度变量
	vec3 viewDir = normalize(viewPos - FragPos); // 视线方向向量
	vec3 reflectDir = reflect(-lightDir, norm);  // 沿着法线的反射向量
	// reflect函数要求第一个向量是从光源指向片段位置的向量，但是lightDir当前正好相反
	float spec = pow(max(dot(viewDir,  reflectDir), 0.0), 32); // 取32次幂，32是高光反光度
	vec3 specular = specularStrength * spec * lightColor; // 镜面反射分量

	vec3 result = (ambient + diffuse + specular) * objectColor;
	FragColor = vec4(result, 1.0);
}