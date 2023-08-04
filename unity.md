# Unity

## UI

- 屏幕显示画面来源

  - Canvas 渲染 UI
    - Screen Space - Overlay：渲染到整个屏幕上（在 Camera 之上）
    - Screen Space - Camera：放置到 Scene World 中离 Camera 固定距离的平面
    - World Space：放置到 Scene World 中的自由平面
  - Cameras 渲染 Scenes
    - Base：渲染到整个屏幕（默认），或渲染到指定 RenderTexture
    - Overlay：渲染到指定 Base Camera 之上（需要加入其 Cameras Stack）

- 长度单位

  - 使用 Unit 作为单位，可当做米(Meter)
  - 开发时，将 Pixel 转换为 Unit（可设置 pixels per unit）
  - 运行时，将 Unit 转换为 Pixel（由 Camera Size 和运行时分辨率决定）

- 高 DPI 缩放问题：

  > Camera 设置 ProjectSetting > Player > Resolution  
  > Canvas 设置 Scale Mode = Scale With Screen Size

  - 设置基础分辨率 W × H
  - 高度变化时，缩放显示画面
  - 宽度变化时，扩充显示画面

- 多分辨率布局问题：

  - Rect Transform: Anchors 实现相对位置与相对大小
  - Layout Component 控制布局

## Game

- Scenes
  - GameObjects
    - Name
    - Tag
    - Layer
    - Components
      - Render
      - Transform
      - Physics
      - Lighting
- Prefabs (Variant)

### Render

- 2D Render Sorting
  - Sorting Layer
  - Order in Layer
  - Distance to Camera（可自定义排序轴）

### Physics

- Joints

  - Distance: 硬杆连接，两端均可旋转
  - Hinge: 硬杆连接，仅主端可旋转（主端有马达）
  - Relative: 硬杆连接，两端均不可旋转（可动态调整距离与角度）
  - Fixed: 硬杆连接，两端均不可旋转（弹性旋转）
  - Spring: 弹簧连接，仅主端可旋转
  - Wheel: 弹簧连接，仅主端可旋转（主端有马达）
  - Slide: 滑轨，固定在对端上（主端有马达）
  - Target: 点向吸引力
  - Friction: 空间阻尼
