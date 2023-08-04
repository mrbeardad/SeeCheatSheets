# Godot

## 基础概念

- Node: 内置基础组件
- Scene: 自定义组件
- Script: 赋予 Scene 逻辑行为
- Viewport: 一块屏幕绘制区域，World2D(Canvas), World3D, Camera

> 用 Scene Editor 编辑的缺点是只能静态组合，不能动态加载

## UI

- 高 DPI 与多分辨率问题：
  - Godot 中像素均为物理像素
  - 通过设置 Stretch Mode 为 canvas_items ，改变分辨率大小会自动缩放或扩充画面
