# embedded-library
基于嵌入式的代码库

[![Language](https://img.shields.io/badge/Language-C%2FC%2B%2B-blue.svg)](https://en.wikipedia.org/wiki/C_(programming_language))
[![License](https://img.shields.io/badge/License-MIT-yellow.svg)](https://opensource.org/licenses/MIT)
[![Status](https://img.shields.io/badge/status-wip-green.svg)](#)
[![GitHub stars](https://img.shields.io/github/stars/your-username/your-repo-name.svg?style=social)](https://github.com/your-username/your-repo-name/stargazers)

这是一个为嵌入式系统精心打造的个人代码库，包含了在项目开发中常用、可复用的模块与算法。旨在提高开发效率，减少重复“造轮子”的工作。

A personal collection of reusable, platform-independent code modules and algorithms for embedded systems (C/C++). Designed to accelerate development and prevent reinventing the wheel.

## ✨ 特点 (Features)

*   📦 **模块化设计 (Modular)**: 每个功能（如PID, H-Bridge）都自成一体，高内聚、低耦合。
*   🔩 **平台无关 (Platform-Independent)**: 核心代码与具体硬件解耦，易于移植到不同MCU平台 (STM32, ESP32, Arduino, etc.)。
*   📝 **文档清晰 (Well-Documented)**: 每个模块都配有清晰的接口注释和独立的 `README.md` 说明其用法。
*   🚀 **易于集成 (Easy to Integrate)**: 只需将对应的 `.c` 和 `.h` 文件复制到你的新项目中即可使用。

## 📂 目录结构 (Directory Structure)
├── algorithms/ # 存放通用算法 (如 PID, Kalman Filter)
├── drivers/ # 存放外设驱动 (如 H-Bridge, OLED)
├── utils/ # 存放通用工具 (如 Key Scan, Circular Buffer)
├── examples/ # 存放完整、可编译的工程示例 (可选)
├── LICENSE # 开源许可证
└── README.md # 你正在阅读的这个文件

## 🚀 快速开始 (Getting Started)

1.  **克隆仓库到本地**:
    ```bash
    git clone git@github.com:your-username/your-repo-name.git
    ```

2.  **找到你需要的模块**:
    例如，如果你需要PID算法，请进入 `algorithms/pid/` 目录。

3.  **集成到你的项目中**:
    将模块的 `.c` 和 `.h` 文件复制到你的项目源码文件夹中，并在需要的地方 `#include "pid.h"`。

4.  **阅读模块文档**:
    每个核心模块文件夹下都包含一个独立的 `README.md` 文件，请务必阅读它以了解详细的API用法、配置和示例。

## 📦 当前可用模块 (Available Modules)

此列表将持续更新。

#### 算法 (Algorithms)
*   **PID 控制器 (`/algorithms/pid`)**
    *   一个通用的增量式/位置式 PID 控制器实现。

#### 驱动 (Drivers)
*   **H桥电机驱动 (`/drivers/h-bridge`)**
    *   一个抽象的H桥驱动，支持正转、反转、刹车和PWM调速。

#### 工具 (Utils)
*   **按键扫描 (`/utils/key-scan`)**
    *   一个高效的按键扫描工具，支持独立按键和矩阵键盘，并集成了去抖、短按、长按、双击等功能。
*   **环形缓冲区 (`/utils/circular-buffer`)**
    *   一个线程安全的环形缓冲区实现，常用于串口数据收发。

---
*... 更多模块正在整理和添加中！*

## 🤝 如何贡献 (How to Contribute)

这是一个个人项目，但非常欢迎提出建议和改进！如果你有好的想法或者发现了Bug，请随时提一个 **Issue**。

如果你想贡献代码：
1.  **Fork** 本仓库
2.  创建你的功能分支 (`git checkout -b feature/AmazingFeature`)
3.  提交你的更改 (`git commit -m 'feat: Add some AmazingFeature'`)
4.  推送到分支 (`git push origin feature/AmazingFeature`)
5.  打开一个 **Pull Request**

## 📄 许可证 (License)

本项目采用 [MIT](https://opensource.org/licenses/MIT) 许可证。详情请见 `LICENSE` 文件。

---