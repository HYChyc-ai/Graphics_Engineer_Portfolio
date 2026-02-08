## 04_OpenGL_Light

这是一个基于 **OpenGL 3.3 core** 的学习型 Demo，参照 LearnOpenGL CN 2.1和2.2节
主要用于理解和实践 **光照模型**，包括 **环境光、漫反射光、镜面反射光** 的应用。

### 主要内容
- 环境光照、漫反射光以及镜面反射光的计算
- 光照模型的实现与应用
- 法线计算和标准化
- 基础着色器管线的运用

---

### Demo效果预览

<p align="center">
  <img src="resources/assets/basic_lighting.gif" width="600"/>
</p>

>> 场景中包括一个立方体和一个光源，演示了不同的光照效果：  
> - 立方体表面应用了环境光、漫反射光和镜面反射光
> - 观察者视角动态调整，观察物体在不同光照下的变化

---

## 核心实现说明

### 1. 环境光
- 使用 `ambientStrength` 来调整环境光强度
- 再与光照相乘得环境光分量 `ambient`

### 2. 漫反射光
- 使用法线与光源方向的点乘计算漫反射光强度 `diff` ，再与光照相乘得漫反射分量 `diffuse`


### 3. 镜面反射光照
- 使用 `specularStrength` 来调整镜面强度
- 将视线向量与反射光向量点乘，再调整高光反光度得到镜面反射强度 `spec`
- 最终 `specularStrength` `spec` 和光照相乘得镜面反射分量
- 
### 4. 着色器部分
- 顶点着色器处理法线、物体位置和光源信息
- 片段着色器计算环境光、漫反射光以及镜面反射光
- 结果颜色通过 `FragColor` 输出到渲染管线


---

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
