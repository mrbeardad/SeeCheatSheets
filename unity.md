# Unity

## UI

- 屏幕显示画面来源

  - Cameras 渲染 Scenes
    - Base：渲染到整个屏幕（默认），或渲染到指定 RenderTexture
    - Overlay：渲染到指定 Base Camera 之上（需要加入其 Cameras Stack）
  - Canvas 渲染 UI
    - Screen Space - Overlay：渲染到整个屏幕（Camera 之上）
    - Screen Space - Camera：放置到 Scene World 中离 Camera 固定距离的平面
    - World Space：放置到 Scene World 中的自由平面

- 长度单位

  - 使用 Unit 作为逻辑单位（可视作 Meter 米）
  - 开发时，将 Pixel 转换为 Unit（可设置 pixels per unit）
  - 运行时，将 Unit 转换为 Pixel（由 Camera Size 和运行时分辨率决定）

- 高 DPI 缩放问题

  - Camera
    - 设置基础分辨率 W × H (Player Setting)
    - 高度变化时，缩放显示画面
    - 宽度变化时，扩充显示画面
  - Canvas
    - Scale Mode: Scale With Screen Size
    - Scale Mode: Constant Physical Size
    - Scale Mode: Constant Pixel Size

- 多分辨率布局问题

  - Camera
    - 见上
  - Canvas
    - Rect Transform: 使用 Anchors 实现相对位置与相对大小
    - Layout Component: 使用布局容器

## Game

- Scenes
  - GameObjects
    - Name: 可读性名称
    - Tag: 分类用于脚本区分
    - Layer: 分类用于内置系统交互
    - Components: 赋予功能
      - Render
      - Transform
      - Physics
      - Lighting
  - Prefabs (Variant)

### Render

#### 2D

- Sprite Renderer

  - Draw Mode
    - Simple: 正常拉伸
    - Sliced: 四角不变，其余正常拉伸
    - Tiled: 四角不变，四边与中间重复渲染代替拉伸

- Tilemap Renderer

- Sprite Mask

  - Visible Inside Mask: 视口
  - Visible Outside Mask: 洞口

- Render Sorting
  1. Sorting Layer
  2. Order in Layer
  3. Distance to Camera（可在 Render Pipeline 中自定义排序轴）

#### Color

- Linear & Gamma (Player Setting)
- HDR Enable (Render Pipeline)
- HDR Output (Player Setting)

#### Texture

- Sprite Editor

  - 2D
    - 分割图集
    - 修改 Border, Pivot, Outline, Physics Shape
    - 编辑骨骼

- Filter Mode
  - Point: 原始像素点，不加任何滤镜
  - Bilinear: 边缘模糊化
  - Trilinear: 边缘模糊化，包括 mipmap

#### Animation

- Animator Controller
  - Layer: 可同时进行的不相干的动画，通常用于控制同一物体不同部位
  - Parameter: 用于脚本控制动画
  - State (Animation Clips): 表示状态的循环动画
  - Transition: 动画切换时的过度动画
    - 仅在特定时间或条件下触发
    - 可选被其他 Transition 中断，排在前面 Transition 具有更高优先级
- 2D Skeleton
  - Sprite Skin
  - Sprit Library
  - Sprite Resolver
  - IK Manager 2D
  - IK Resolver 2D
- Target Matching

#### Lighting

#### Particles

- Main
  - Play On Awake
  - Duration
  - Looping
  - Start Lifetime
  - Start Speed
  - Start Rotation
  - Start Size
  - Start Color
  - Gravity
  - Simulate Space
  - Max Particles
- Emission
  - Rate Over Time
  - Rate Over Distance
  - Bursts
- Shape
  - Shape
  - Radius Thickness: 0 表示仅从表面发射，1 表示从整个体积发射
  - Arc: 可发射的弧度
    - Mode
      - Random: 随机发射
      - Loop: 旋转发射
      - Ping: 旋转发射（下次循环反向）
      - Burst: 使 Burst 发射相对 Random 分布更加均匀
    - Spread: 旋转间隔，0 表示无间隔，0.1 表示间隔 10%
    - Speed: 旋转速度，1 表示每秒旋转 1 圈
  - Align To Direction: 粒子 Rotation 根据初始速度方向设置
  - Randomize Direction: 将随机发射方向与默认发射方向混合
  - Spherize Direction: 将表面法线发射方向与默认方向混合
  - Randomize Position: 扩张发射位置

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
