# Unreal Engine

## Editor

### UI

![ue_ui](images/ue_ui.png)

**Viewport 导航**

- `LMB + drag`: 前后移动 / 左右旋转
- `MMB + drag`: 平移镜头
- `RMB + drag`: 自由旋转镜头
- `RMB + W/A/S/D`: WASD 飞行模式
- `RMB + Q/E`: 下降 / 上升
- `RMB + C/Z`: 拉远 / 拉近 FOV
- `F`: 聚焦选中对象
- `Alt + LMB + drag`: 围绕枢轴旋转
- `Alt + MMB + drag`: 平移
- `Alt + RMB + drag`: 推拉缩放

**变换工具**

- `Q`: 选择
- `W`: 移动
- `E`: 旋转
- `R`: 缩放
- `Space`: 切换变换工具（Move / Rotate / Scale）
- `V`: 顶点吸附（移动时按住）
- `End`: 吸附到地面

**视图切换**

- `G`: Game View，隐藏 editor-only 可视元素
- `Alt + 2`: Wireframe
- `Alt + 3`: Unlit
- `Alt + 4`: Lit
- `Ctrl + R`: 切换 Realtime
- `F11`: Viewport 全屏

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
- `Ctrl + C/V/X`: 复制 / 粘贴 / 剪切
- `Ctrl + D`: Duplicate
- `Delete`: 删除
- `H`: 隐藏选中
- `Ctrl + H`: 显示全部
- `Ctrl + B`: 在 Content Browser 中定位
- `Ctrl + E`: 打开选中资源 / 编辑相关资产
- `Ctrl + G`: Group
- `Shift + G`: Ungroup

### Blueprint

- 蓝图可视作一个高级 C++ Class, 继承自某个 Base Class
- 不同于 C++ Class 用 UPROPERTY 和 UFUNCTION 封装数据与逻辑, 蓝图用 Event Graph 同时封装数据与逻辑
- UE 中的 Class 基于原型设计模式, 可以将 CDO 实例视作一个资产
- UE 中很多数据的拷贝是浅拷贝(引用), 特别是大多数资产(通过 Path 引用)

## C++

UE 用 C++ 扩展出接近动态语言的开发体验，核心是 `UObject`、Reflection、Serialization、GC、Blueprint、Network Replication。
原生 C++ 和 `UObject` 体系可以混用，但生命周期、指针和构造规则不同。

> [Epic C++ Coding Standard](https://dev.epicgames.com/documentation/en-us/unreal-engine/epic-cplusplus-coding-standard-for-unreal-engine)

### Reflection

UE Reflection 由 `UnrealHeaderTool` 解析宏生成元数据，不是标准 C++ RTTI。

| Macro          | 用途                                       |
| -------------- | ------------------------------------------ |
| `UCLASS()`     | 声明 `UObject` 派生类                      |
| `UINTERFACE()` | 声明 UE Interface                          |
| `USTRUCT()`    | 声明可反射结构体                           |
| `UENUM()`      | 声明可反射枚举                             |
| `UPROPERTY()`  | 属性暴露、序列化、GC 引用跟踪、Replication |
| `UFUNCTION()`  | 函数暴露、RPC、Blueprint 调用              |
| `UPARAM()`     | 修饰函数参数                               |
| `UDELEGATE()`  | 声明 delegate 类型                         |

常见 specifier：

- `BlueprintType`: 类型可作为 Blueprint 变量
- `Blueprintable`: 类可被 Blueprint 继承
- `BlueprintReadOnly` / `BlueprintReadWrite`: 属性可读 / 可写
- `EditDefaultsOnly`: 只在 Class Default / Blueprint 默认值中编辑
- `EditInstanceOnly`: 只在 instance 上编辑
- `EditAnywhere`: 默认值和 instance 都可编辑
- `VisibleAnywhere`: 可见但不可编辑
- `Transient`: 不序列化到磁盘
- `Replicated`: 属性参与网络复制

### UObject

`UObject` 提供：

- Reflection / metadata
- Serialization / asset loading
- Garbage Collection
- Network Replication 基础能力
- Blueprint 暴露与调用
- Class Default Object（`CDO`）和默认属性系统

**构造**

- `CreateDefaultSubobject<T>()`: 只在构造函数中创建 default subobject，常用于 `UActorComponent`
- `NewObject<T>()`: 运行时创建 `UObject`
- `SpawnActor<T>()`: 运行时创建 `AActor`，由 `UWorld` 管理
- `LoadObject<T>()` / `StaticLoadObject()`: 从 object path 加载资产
- `.uasset` / `.umap` 本质上是 `UPackage` 序列化结果

**生命周期**

- C++ constructor: 初始化 CDO 和 default subobject，不要依赖 `World`
- `PostInitProperties()`: 属性初始化后
- `PostLoad()`: 从磁盘反序列化后
- `BeginPlay()`: 进入 gameplay, 最好在这里去检测 Editor 设置的属性
- `Tick()`: 每帧更新，需显式开启
- `EndPlay()` / `Destroyed()`: Actor 退出世界或销毁

**GC**

- UE GC 是 mark-sweep，不是普通引用计数
- 从 Root Set 出发，沿 `UPROPERTY` / reflected container 等可见引用标记对象
- 未被标记的 `UObject` 会在 GC 时回收，GC 通常在帧间执行
- `AddToRoot()` / `RemoveFromRoot()` 可手动加入 Root Set，慎用
- `AActor` 的生命周期主要由 `UWorld` 管理，销毁用 `Destroy()`
- `UActorComponent` 生命周期通常跟随 owner `AActor`

**指针规则**

| 类型                              | 用途                                            |
| --------------------------------- | ----------------------------------------------- |
| `TObjectPtr<T>`                   | UE5 推荐的 `UObject` 成员引用，配合 `UPROPERTY` |
| `TWeakObjectPtr<T>`               | 弱引用 `UObject`，对象销毁后可检测失效          |
| `TSoftObjectPtr<T>`               | 软引用资产，记录路径，可延迟加载                |
| `TSubclassOf<T>`                  | 限制 Class 类型必须继承自 `T`                   |
| `TStrongObjectPtr<T>`             | 非 `UPROPERTY` 场景下强持有 `UObject`           |
| `TSharedPtr<T>` / `TSharedRef<T>` | 管理非 `UObject` 的原生 C++ 对象                |

**Outer vs Owner**

| 概念    | 作用域    | 核心含义                          | 典型用途                           |
| ------- | --------- | --------------------------------- | ---------------------------------- |
| `Outer` | `UObject` | 这个 object 在哪个 object 下面    | object path、package、序列化、查找 |
| `Owner` | `AActor`  | 这个 Actor 归哪个 Actor 负责/控制 | RPC、network ownership、relevancy  |

```cpp
UMyObject* Obj = NewObject<UMyObject>(this); // this 是 Obj 的 Outer
WeaponActor->SetOwner(PlayerController);     // PlayerController 是 WeaponActor 的 Owner
```

- `Outer` 不等于 `Owner`；`Owner` 不会改变 object path / Outer
- `Owner` 也不等于 attach parent；transform 层级看 `AttachToComponent()` / `RootComponent`

### Replication

- Replication 只从 server 同步到 client
- `UPROPERTY(Replicated)`: 同步属性，需在 `GetLifetimeReplicatedProps()` 注册
- `ReplicatedUsing=OnRep_X`: client 收到属性更新后回调
- RPC 本质是通过网络调用 `UFUNCTION`
- `Server` RPC: client 请求 server 执行，常用于输入意图
- `Client` RPC: server 指定某个 owning client 执行
- `NetMulticast` RPC: server 广播到相关 client
- `GameMode` 不复制；跨端状态放 `GameState` / `PlayerState`

### Modules

典型项目结构：

```text
MyProject/
  Source/
    MyProject.Target.cs
    MyProjectEditor.Target.cs
    MyProject/
      MyProject.Build.cs
      Public/
      Private/
    MyModule/
      MyModule.Build.cs
      Public/
        MyModule.h
      Private/
        MyModule.cpp
```

`*.Build.cs` 关键点：

- `PublicDependencyModuleNames`: Public header 需要暴露的依赖
- `PrivateDependencyModuleNames`: 只在 Private 实现中使用的依赖
- `Public/`: 对其他 module 暴露
- `Private/`: module 内部实现

配置文件优先级参考：[Configuration File Hierarchy](https://dev.epicgames.com/documentation/en-us/unreal-engine/configuration-files-in-unreal-engine#configurationfilehierarchy)

## Gameplay Framework

### 对象层级

**Engine-level singletons**

- `UEngine`: global engine instance
- `UGameInstance`: 跨 level 存活，适合保存会话级状态
- `ULocalPlayer`: 本地玩家入口，split screen 时可有多个

**Gameplay framework**

- `AGameMode`: server-only，定义规则、spawn、胜负等
- `AGameSession`: server-only，登录、会话、匹配相关
- `AGameState`: replicated，全局游戏状态
- `APlayerState`: replicated，玩家状态，通常跨 pawn 存活
- `APlayerController`: 玩家输入与控制入口
- `APawn` / `ACharacter`: 可被 controller possessed 的实体

**Per-world objects**

- `UWorld`: 一个运行中的世界
- `ULevel`: `World` 中的关卡数据块
- `AWorldSettings`: level 级配置
- `ALevelScriptActor`: Level Blueprint 的运行时实例

**World entities**

- `AActor`: 世界中的实体，有 transform，可 spawn / destroy
- `UActorComponent`: Actor 的行为模块，无独立 transform
- `USceneComponent`: 带 transform 的 component，可组成层级

### 运行时关系

- `GameMode` 只在 server 存在，client 用 `GameState` 获取同步后的规则状态
- `PlayerController` 不等于角色本体，角色本体通常是 `Pawn` / `Character`
- `Pawn` 被 `Controller` possess 后才有控制来源
- `ActorComponent` 用组合拆行为，避免 `Actor` 继承层级膨胀

## Assets And Paths

### 路径概念

- **物理文件路径**：磁盘真实路径，例如 `Content/Characters/Hero.uasset`
- **打包后路径**：Cook / Package 后仍保留逻辑目录，但物理上可能在 `.pak` / `.ucas`
- **Mount Point**：虚拟挂载点，例如项目 `Content/` 通常挂载到 `/Game/`
- **Long Package Name**：不带扩展名的 package 路径，例如 `/Game/Characters/Hero`
- **Object Path**：对象完整路径，例如 `/Game/Characters/Hero.Hero`
- **Subobject Path**：非 top-level object 路径，通常在 root object 后用 `:` 接 subpath，例如 `/Game/Maps/Main.Main:PersistentLevel.Actor_0.Component`
- **Class Path**：Blueprint 生成类路径常见形式，例如 `/Game/BP_Hero.BP_Hero_C`

### 名称 API

- `GetName()`: 对象自身名字，不含 Outer / Package，例如 `Hero`
- `GetPathName()`: 包含 Outer 链；top-level object 通常是 `/Package/Asset.Asset`，非根对象通常带 `:SubPath`
- `GetFullName()`: `ClassName + " " + GetPathName()`，例如 `SkeletalMesh /Game/Characters/Hero.Hero`
- `GetOuter()`: 所属外层对象，资源对象的 Outer 通常是 `UPackage`

```text
Content/Characters/Hero.uasset          # 物理文件路径
/Game/Characters/Hero                   # Long Package Name
Hero                                    # GetName()
/Game/Characters/Hero.Hero              # Object Path / GetPathName()
SkeletalMesh /Game/Characters/Hero.Hero # GetFullName()
/Game/Maps/Main.Main:PersistentLevel.Actor_0.Component # subobject path
```

### 引用方式

- Hard Reference: 直接引用资产，加载当前对象时会连带加载依赖
- Soft Reference: 只记录路径，需要时异步 / 手动加载，适合大资源和可选资源
- `ConstructorHelpers::FObjectFinder`: 构造期硬查找资产，只适合默认资源绑定
- `TSoftObjectPtr::LoadSynchronous()`: 同步加载软引用，简单但可能卡顿
- `StreamableManager`: 异步加载软引用资源
