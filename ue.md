# Unreal Engine

## Editor

### Level Editor

**Viewport 导航**

- `LMB + drag`: 前后移动 / 左右旋转
- `MMB + drag`: 平移镜头
- `RMB + drag`: 自由旋转镜头
- `RMB + W/A/S/D`: WASD 飞行模式
- `RMB + Q/E`: 上升/下降
- `RMB + C/Z`: 拉远/拉近 FOV
- `F`: 聚焦选中对象
- `Alt + LMB + drag`: 围绕枢轴旋转
- `Alt + RMB + drag`: 推拉缩放
- `Alt + MMB + drag`: 平移

**变换工具**

- `Q`: 选择
- `W`: 移动
- `E`: 旋转
- `R`: 缩放
- `Space`: 切换变换工具 (Move/Rotate/Scale)
- `V`: 顶点吸附 (移动时)
- `End`: 吸附到地面

**视图切换**

- `G`: 游戏视图/无光照模式
- `Ctrl + R`: 切换实时播放效果
- `F11`: 视口全屏

**选择**

- `LMB click`: 选中 Actor
- `Ctrl + LMB click`: 切换选中 / 取消选中
- `Shift + LMB click`: 追加选中
- `Ctrl + Alt + LMB drag`: 框选
- `Ctrl + A`: 全选
- `Esc`: 取消全部选中
- `Ctrl + Alt + A`: 选中所有同类型 Actor

**编辑**

- `Ctrl + Z`: 撤销
- `Ctrl + Y`: 重做
- `Ctrl + C/V/X`: 复制/粘贴/剪切
- `Ctrl + D`: 复制选中对象
- `Delete`: 删除
- `H`: 隐藏选中
- `Ctrl + H`: 显示全部
- `Ctrl + B`: 在内容浏览器中查找
- `Ctrl + E`: 编辑选中 Actor
- `Ctrl + G`: 编组
- `Shift + G`: 解组

## C++

UE 将 C++ 改造得接近动态语言（比如 C#）以简化编程的同时保证性能。
这些基础功能都由 `UObject` 提供，用户可以自由混合使用该系统和原生 C++

- 垃圾回收
  - 构造
    - `CreateDefaultSubobject<class>`：构造函数中调用
    - `NewObject<class>`：运行时调用
    - 构造采用原型模式，构造函数用于初始化 CDO
    - 对象也可以直接从硬盘文件反序列化得到（`UPackage` <=> `.uasset`/`.umap`）
  - 销毁
    - 强引用计数归零时，会在下次 GC 回收，GC 仅在帧间运行
    - 强引用：Root Set, `UProperty`, `TArray`, `TStrongObjectPtr` 等
    - `AActor` 和 `UActorComponent` 在销毁时会将它们的强引用置空
- 反射系统
- 序列化
- 自动初始化
- 网络复制
  - 同步属性
  - 远程方法

宏定义，用于辅助定义类型

```cpp
UCLASS()
UINTERFACE()
USTRUCT()
UENUM()
UPROPERTY()
UFUNCTION()
UPARAM()
UDELEGATE()
```

基础类型

```cpp
// 基础类型
UObject

UClass
TSubclassOf

TArray
TMap
TSet
TStrongObjectPtr

// 智能指针用于管理非 UObject 对象
TSharedRef
TSharedPtr
TWeakPtr
```

> [命名规则](https://dev.epicgames.com/documentation/en-us/unreal-engine/epic-cplusplus-coding-standard-for-unreal-engine)

引擎启动

1. 初始化核心子系统，它们可能由 Native 或 UObject 实现
2. 加载项目模块
3. 初始化 UObject 对象

项目结构

- MyProject
  - Source
    - MyProject.Target.cs
    - MyProjectEditor.Target.cs
    - MyProject (Primary game module folder)
      - MyProject.Build.cs
      - Other C++ classes in your game module
    - MyModule
      - Public
        - MyModule.h
      - Private
        - MyModule.cpp
      - MyModule.Build.cs

配置顺序
https://dev.epicgames.com/documentation/en-us/unreal-engine/configuration-files-in-unreal-engine#configurationfilehierarchy

## UObjects

Engine-level singletons

- `UEngine` → global engine instance
- `UGameInstance` → persists across levels

Per-world objects

- `UWorld` → represents the game world
- `ULevel` → contains actors

Gameplay framework (per world)

- `AGameMode` (server-only)
- `AGameState`
- `APlayerController`
- `APawn` / `ACharacter`
- `APlayerState`

Everything else

- `AActor` → entities in the world
- `UActorComponent` → behavior modules
- Assets (`UStaticMesh`, `UMaterial`, etc.)
