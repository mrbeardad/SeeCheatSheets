# Godot

## 基础概念

- Node: 内置基础组件
- Scene: 自定义组件
- Script: 赋予 Scene 逻辑行为
- Viewport: 一块屏幕绘制区域，World2D(Canvas), World3D, Camera

> 用 Scene Editor 编辑的缺点是只能静态组合，不能动态加载

## 分辨率

- Godot 中像素均为物理像素
- 通过设置 Stretch Mode 为 canvas_items 会自动缩放分辨率，以达到类似逻辑像素的效果

## 数学

- Vector: relative direction and magnitude
  - Point: the Vector from Origin to Point
  - Line: the Vector from Point to Point
  - Plane: the Vector for Normal
