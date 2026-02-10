## 05_OpenGL_Advanced_Light

这是一个基于 **OpenGL 3.3 core** 的学习型 Demo，参照 LearnOpenGL CN  
**第二章 光照（基础光照 / 材质 / 光照贴图 / 多光源 / 聚光灯）** 内容完成。

本 Demo 在片段着色器中实现了 **点光源 + 聚光灯（SpotLight）** 的组合光照模型，
并使用 **Phong 光照模型** 将光照拆分为  
**环境光（Ambient） / 漫反射（Diffuse） / 镜面反射（Specular）** 三个部分分别计算后叠加。

---

## 主要内容

- Phong 光照模型完整实现
- 材质（Material）与光源（Light）结构体设计
- 点光源（Point Light）
- 聚光灯（Spot Light，摄像机手电筒效果）
- 聚光灯平滑边缘（Soft Edge）
- 多光源光照结果叠加
- 法线矩阵（Normal Matrix）解决旋转导致的高光错误
- 动态光源运动（绕物体旋转并上下起伏）

---

## Demo 效果预览

<p align="center">
  <img src="resources/assets/advanced_lighting.gif" width="600"/>
</p>

> 场景说明：
> - 场景中包含多个自转的立方体
> - 一个点光源绕物体旋转并上下起伏
> - 摄像机同时作为聚光灯（手电筒效果）
> - 聚光灯具有平滑边缘，避免生硬的光照边界

---

## 核心实现说明

### 1. 材质结构体（Material）

```glsl
struct Material {
    sampler2D diffuse;
    vec3      specular;
    float     shininess;
};
```
- `diffuse` 使用漫反射贴图，颜色信息由纹理提供
- `specular` 控制镜面反射强度
- `shininess` 控制高光集中程度

### 2. 点光源与聚光灯结构体
```glsl
struct PointLight {
    vec3 position;

    vec3 ambient;
    vec3 diffuse;
    vec3 specular;

    float constant;
    float linear;
    float quadratic;
};

struct SpotLight {
    vec3 position;
    vec3 direction;

    float cutOff;
    float outerCutOff;

    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};
```
- 点光源用于提供场景的基础动态光照
- 聚光灯绑定摄像机，实现“手电筒”效果
- 使用 `cutOff / outerCutOff` 实现平滑边缘

### 3. 法线矩阵（Normal Matrix）
```cpp
glm::mat3 normalMatrix = glm::transpose(glm::inverse(glm::mat3(model)));
lightingShader.setMat3("normalMatrix", normalMatrix);
```
```glsl
Normal = normalize(normalMatrix * aNormal);
```
- 解决模型旋转、非等比缩放导致的法线方向错误
- 保证镜面反射在物体旋转时依然正确

### 4. 聚光灯平滑边缘（Soft Edge）
```glsl
float theta = dot(lightDir, normalize(-light.direction));
float epsilon = light.cutOff - light.outerCutOff;
float intensity = clamp((theta - light.outerCutOff) / epsilon, 0.0, 1.0);

ambient  *= intensity;
diffuse  *= intensity;
specular *= intensity;
```
- 避免硬切割的聚光边缘
- 聚光中心亮度最高，边缘逐渐衰减

### 5. 多光源叠加
```glsl
vec3 result = vec3(0.0);

result += CalcPointLight(pointLight, normal, FragPos, viewDir);
result += CalcSpotLight(spotLight,  normal, FragPos, viewDir);

FragColor = vec4(result, 1.0);
```
- 每种光源独立计算自己的光照贡献
- 最终结果统一叠加，便于扩展更多光源类型

## 基础交互说明（示例）

- **W / A / S / D**：前后左右移动摄像机  
- **鼠标移动**：控制视角方向  
- **滚轮**：缩放视野（FOV）

---

## 使用的技术栈

- **OpenGL 3.3 Core Profile**
- **GLFW**
- **GLAD**
- **GLM**

---

### build
```bash
git clone https://github.com/HYChyc-ai/Graphics_Engineer_Portfolio.git
cd Graphics_Engineer_Portfolio
mkdir build && cd build
cmake ..
