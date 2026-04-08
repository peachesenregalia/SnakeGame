# SnakeGame

基于控制台的贪吃蛇游戏，使用C++开发，支持存档系统。

## 功能特性
- 基础游戏机制：移动、吃食物增长、碰撞检测
- 存档/读档：保存蛇身、方向、食物位置、分数
- 双缓冲渲染：避免屏幕闪烁
- 非阻塞输入：支持键盘控制

## 构建要求
- CMake 3.10+
- C++17 编译器（MSVC 或 GCC）

## 构建步骤
```bash
mkdir build
cd build
cmake ..
cmake --build . --config Release
```

## 运行
```bash
./SnakeGame.exe  # Windows
```

## 控制
- WASD 或方向键：移动
- P：暂停
- S：保存
- L：加载
- Q：退出

## 项目结构
- `Game.h/cpp`：主游戏循环
- `Snake.h/cpp`：蛇逻辑
- `Renderer.h/cpp`：渲染
- `Input.h/cpp`：输入处理
- `SaveSystem.h/cpp`：存档系统
- `Food.h/cpp`：食物
- `CMakeLists.txt`：构建配置

## AI辅助开发
本项目使用GitHub Copilot生成部分代码，人工优化和调试。

## 许可证
MIT License