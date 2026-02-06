## 02_OpenGL_3D_Space

这是一个基于 **OpenGL 3.3 core** 的学习型 Demo，参照 LearnOpenGL CN 1.8节
主要用于理解和实践 **Model / View / Projection 变换**

### 主要内容
- 使用stb_image加载纹理
- 纹理环绕与过滤方式
- 结合基础着色器管线
- 基本图像坐标变换：平移，旋转，放缩

---

### Demo效果预览

<p align="center">
  <img src="resources/result/rotating_cubes.gif" width="600"/>
</p>

> 场景中包含多个立方体：
> - 所有物体基于统一时间系统进行动画
> - 每个立方体具有不同的旋转轴与相位偏移

## Demo 目标

- 理解 **Model / View / Projection** 矩阵在渲染管线中的作用
- 掌握 **基于时间的动画（Time-based Animation）**
- 实现 **多个物体的独立变换**

---

## 核心实现说明

### 1️. 多物体模型变换（Model Matrix）
- 每个立方体拥有独立的 `model` 矩阵
- 使用 `glm::translate` 和 `glm::rotate` 进行组合变换
- 通过时间函数实现连续旋转动画

### 2️. 时间驱动动画
- 使用 `glfwGetTime()` 作为统一时间源
- 避免使用 `i + 1` 等方式直接影响角速度
- 通过 **相位偏移** 区分不同物体的动画状态

## 使用的技术栈

- **OpenGL 3.3 Core Profile**
- **GLFW**
- **GLAD**
- **GLM**

### build
```bash
git clone https://github.com/HYChyc-ai/Graphics_Engineer_Portfolio.git
cd Graphics_Engineer_Portfolio
mkdir build && cd build
cmake ..
