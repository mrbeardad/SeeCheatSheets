# Flame Engine

Game UI 与 App UI 的区别：

- Game UI 命令式，App UI 声明式
- Game UI 实时渲染，App UI 仅在状态更新时渲染

Game 概念

- Camera
  - Viewport：观察世界的窗口本身
  - Viewfinder：控制窗口如何观察世界，如位置、缩放等
- World
  - Tile：地形
  - Sprite：角色
  - Effect：特效动画
  - Particle：粒子动画
- Forge2D
  - Body
    - 基本状态
      - position
      - angle
    - 运动状态
      - linear velocity
      - angular velocity
    - 运动参数
      - type
        - static
        - kinematic
        - dynamic
        - bullet(CCD)
      - Fixture
        - shape
        - density
        - friction
        - restitution
        - filtering
        - sensor
      - linear damping
      - angular damping
      - Mass Data：通常根据 Fixture 的 Shape 和 Density 自动设置
        - mass
        - center of mass
        - rotational inertia
    - 状态转移
      - Force
      - Torque
      - Linear Impulse
      - Angular Impulse
