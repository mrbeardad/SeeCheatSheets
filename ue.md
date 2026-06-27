# Unreal Engine

## Project Structure

> [Epic C++ Coding Standard](https://dev.epicgames.com/documentation/en-us/unreal-engine/epic-cplusplus-coding-standard-for-unreal-engine)

UE 项目结构围绕 project descriptor、module、target、asset 目录组织：

- `.uproject`: project descriptor，记录 module、plugin、engine association 等
- `Source/`: C++ source，按 module 拆分
- `Content/`: `.uasset` / `.umap` 等项目资产，通常挂载到 `/Game/`
- `Config/`: `.ini` 配置，按层级覆盖
- `Plugins/`: 项目级 plugin，每个 plugin 可有自己的 `Source/`、`Content/`
- `Intermediate/`: UBT / UHT 生成的中间文件，不应依赖或提交
- `Binaries/`: 编译产物，通常不提交
- `Saved/`: log、autosave、local config、crash data，通常不提交
- `DerivedDataCache/`: shader、texture 等派生缓存，可再生成

关键边界：

| 概念    | 文件                     | 作用                                        |
| ------- | ------------------------ | ------------------------------------------- |
| Project | `.uproject`              | 项目描述，声明 module、plugin、Engine 版本  |
| Plugin  | `.uplugin`               | 可复用功能包，可包含 code module 和资产     |
| Target  | `*.Target.cs`            | 最终构建目标，如 Game / Editor / Server     |
| Module  | `*.Build.cs` + `Source/` | C++ 编译、链接、加载单元                    |
| Package | `.uasset` / `.umap`      | 资产序列化单元，运行时通过 object path 引用 |

典型 C++ 项目结构：

```text
MyProject/
  MyProject.uproject
  Config/
    DefaultEngine.ini
    DefaultGame.ini
  Content/
  Source/
    MyProject.Target.cs
    MyProjectEditor.Target.cs
    MyProject/
      MyProject.Build.cs
      Public/
      Private/
    MyProjectEditor/
      MyProjectEditor.Build.cs
      Private/
  Plugins/
    MyPlugin/
      MyPlugin.uplugin
      Source/
        MyPlugin/
        MyPluginEditor/
```

### Target

`Target` 定义最终构建目标，决定编译哪些 module、以什么规则编译。

- `MyProject.Target.cs`: game/runtime target
- `MyProjectEditor.Target.cs`: editor target，可包含 Editor module
- 常见 target type: `Game`、`Editor`、`Client`、`Server`
- `Target.cs` 管 project-level build；`Build.cs` 管 module-level build
- `Target` 通常对应最终可执行构建产物；Windows game / client / server target 常见产物是 `.exe`
- Editor target 通常不是生成一个独立项目 editor `.exe`，而是构建供 `UnrealEditor.exe` 加载的 editor/game module `.dll`
- packaged monolithic game 中，target 会把需要的 module 链接进最终 `.exe`

### Module

`Module` 是 UE 的 C++ 编译、链接和加载单元，不是 C++20 `module`，也不是 namespace。
项目和 plugin 都可以包含多个 module。

| 关注点       | 规则                                                                    |
| ------------ | ----------------------------------------------------------------------- |
| 类型         | Runtime module 进 packaged build；Editor module 只放 editor-only 代码   |
| `Build.cs`   | 给 UBT 声明依赖、include path、编译选项                                 |
| public 依赖  | public header 暴露依赖类型 / 函数时用 `PublicDependencyModuleNames`     |
| private 依赖 | 只在 `.cpp` 或 private header 中使用时用 `PrivateDependencyModuleNames` |
| 目录         | `Public/` 给其他 module include；`Private/` 放内部实现                  |
| editor 依赖  | runtime module 不应依赖 `UnrealEd`；复杂 editor 逻辑拆 Editor module    |

加载入口：

- 默认 game module 通常用 `IMPLEMENT_PRIMARY_GAME_MODULE`
- 普通 module 可实现 `IModuleInterface`，在 `StartupModule()` / `ShutdownModule()` 注册和清理扩展点

编译产物不固定等于一个 `.dll` 或 `.lib`：

- Editor / modular build: 常见为 `UnrealEditor-MyModule.dll`
- Windows `.lib`: 多数是 import library，用于链接 `.dll`
- monolithic build: 多个 module 静态链接进最终 `.exe`

## Runtime System

UE Runtime 的核心不是普通 C++ class hierarchy，而是 `UObject`、Reflection、Serialization、GC、Blueprint、Assets 共同组成的对象系统。
原生 C++ 和 `UObject` 体系可以混用，但构造规则、生命周期、指针语义和反射可见性不同。
理解 UE 代码时先判断对象属于普通 C++、`UObject`、`AActor` / `UActorComponent`，再判断它由谁创建、持有、序列化和回收。

### UObject

`UObject` 是 UE runtime object model 的基础，提供：

- Reflection / metadata
- Serialization / asset loading
- Garbage Collection
- Network Replication 基础能力
- Blueprint 暴露与调用
- Class Default Object（`CDO`）和默认属性系统

#### 反射

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

反射宏不只是“让 UE 看见 C++ 类型”，也是 C++ / Blueprint bridge 的基础：C++ 定义稳定协议、核心规则和状态转移，Blueprint 负责装配、默认值、表现反馈和关卡 glue。

常见 type / class specifier：

| Specifier          | 作用                      | 常见用途 / 注意                        |
| ------------------ | ------------------------- | -------------------------------------- |
| `BlueprintType`    | 类型可作为 Blueprint 变量 | 常用于 `USTRUCT`、`UENUM`、data object |
| `Blueprintable`    | class 可被 Blueprint 继承 | 常用于 C++ gameplay base class         |
| `Abstract`         | class 不能直接实例化      | 只作为基类或协议层                     |
| `NotBlueprintable` | 禁止 Blueprint 继承       | 底层类或不希望 BP 扩展的类型           |

常见 `UPROPERTY` specifier：

| Specifier                          | 作用                                 | 用途 / 注意                                 |
| ---------------------------------- | ------------------------------------ | ------------------------------------------- |
| `EditDefaultsOnly`                 | 只在 class default / BP 默认值中编辑 | 类型级参数，例如 `MaxHealth`、`AttackRange` |
| `EditInstanceOnly`                 | 只在 placed instance 上编辑          | 关卡特例，例如某扇门的 `RequiredKeyId`      |
| `EditAnywhere`                     | 默认值和 instance 都可编辑           | 方便但边界更松，避免滥用                    |
| `VisibleAnywhere`                  | Details 可见但不可编辑               | runtime state 或组件引用                    |
| `BlueprintReadOnly`                | Blueprint 可读不可写                 | 状态给 BP 展示；修改优先走 C++ API          |
| `BlueprintReadWrite`               | Blueprint 可读写                     | 慎用；容易绕过状态修改副作用                |
| `Transient`                        | 不序列化到磁盘                       | runtime cache / 临时对象引用                |
| `Replicated`                       | 属性参与网络复制                     | 需在 `GetLifetimeReplicatedProps()` 注册    |
| `ReplicatedUsing=OnRep_X`          | 复制后在 client 调回调               | UI / 表现响应复制状态变化                   |
| `meta=(ExposeOnSpawn="true")`      | spawn 节点暴露初始化参数             | projectile damage、spawn config、item id    |
| `meta=(AllowPrivateAccess="true")` | private property 仍可被反射暴露      | 常配合 private component pointer            |

常见 `UFUNCTION` / delegate specifier：

| Specifier                     | 作用                                | 用途 / 注意                           |
| ----------------------------- | ----------------------------------- | ------------------------------------- |
| `BlueprintCallable`           | Blueprint 可调用 C++ 函数           | 有执行 pin，适合命令式操作            |
| `BlueprintPure`               | Blueprint 纯读取函数                | 不应修改状态、spawn、播放音效         |
| `BlueprintImplementableEvent` | C++ 声明、Blueprint 实现            | 表现钩子；BP 不实现也不应影响核心规则 |
| `BlueprintNativeEvent`        | C++ 有默认实现，Blueprint 可覆盖    | 默认实现写 `_Implementation()`        |
| `BlueprintAssignable`         | Blueprint 可绑定 multicast delegate | 常用于 `OnHealthChanged`、`OnOpened`  |
| `BlueprintAuthorityOnly`      | 只应在 authority 端执行             | 网络规则提示，不等于完整安全边界      |

#### 构造

**构造途径**

| 目标                    | API                                                 | 与 `CDO` / template 的关系                                                                 | 要点                                                                  |
| ----------------------- | --------------------------------------------------- | ------------------------------------------------------------------------------------------ | --------------------------------------------------------------------- |
| Class Default Object    | engine 内部创建 `UClass` 默认对象                   | `CDO` 是默认值来源；构造它本身时不会“拷贝自己”，native class 通常以父类 `CDO` 为 archetype | 每个 `UClass` 一个，Blueprint 默认值最终落到 generated class 的 `CDO` |
| default subobject       | `CreateDefaultSubobject<T>()`                       | 创建 default subobject template；不是简单“从 owner CDO 拷贝一个普通对象”                   | 只在 constructor 中用，成为 class 默认结构                            |
| 普通 `UObject`          | `NewObject<T>(Outer)`                               | 未传 `Template` 时通常以 `Class` 的 `CDO` 初始化属性；传入 `Template` 时复制该 template    | 运行时创建，必须考虑 `Outer` 和 GC 可达性                             |
| `AActor`                | `UWorld::SpawnActor<T>()`                           | 默认以 Actor class 的 `CDO` / archetype 初始化；`FActorSpawnParameters::Template` 可覆盖   | Actor 由 `UWorld` 管理，不用 `NewObject`                              |
| 延迟初始化 `AActor`     | `SpawnActorDeferred<T>()` + `FinishSpawningActor()` | 初始对象仍来自 `CDO` / template；延迟的是 construction script / component 初始化等后续阶段 | spawn 后、construction 前设置参数                                     |
| 加载已有 asset / object | `LoadObject<T>()` / `StaticLoadObject()`            | 不能简单理解为只拷贝 `CDO`；加载对象会先有默认初始化，再用 package 序列化数据覆盖          | 从 object path 反序列化对象                                           |
| 复制已有 object         | `DuplicateObject<T>()`                              | 主要复制源对象及 subobject 图；源对象才是 template，不是目标 class 的 `CDO`                | 复制 object / subobject 图                                            |

- 不要用 `new` / `delete` 管理 `UObject`
- “从 `CDO` 拷贝”主要指 reflected property 默认值初始化；普通 C++ 成员仍按 C++ 构造规则处理
- UE 构造底层更通用的概念是 `Template` / `Archetype`；没有显式 template 时，运行时新建对象通常退回到 class `CDO`
- `.uasset` / `.umap` 本质上是 `UPackage` 序列化结果，加载路径会走反序列化而不是普通构造

**构造流程**

| 函数 / 阶段                   | 触发场景                                   | 备注                                                                                                                                               |
| ----------------------------- | ------------------------------------------ | -------------------------------------------------------------------------------------------------------------------------------------------------- |
| C++ constructor               | `CDO`、spawn、load 等对象创建路径          | 设置 native default / default subobject；不要假设 `this` 是 gameplay instance                                                                      |
| `PostInitProperties()`        | constructor 后、属性初始化后               | 通用 `UObject` 初始化函数；CDO 创建也会走到这里，修改属性可能影响 class 默认状态                                                                   |
| Blueprint Class Defaults 应用 | `UBlueprintGeneratedClass` CDO 创建 / 重建 | 不是常规 gameplay 回调；在 native constructor / `PostInitProperties()` 基础默认值之后应用 Blueprint 默认值 / component templates；CDO 至此生成完毕 |
| `PostLoad()`                  | 从磁盘 / package 反序列化后                | load path；和 `PostActorCreated()` 互斥                                                                                                            |
| `PostActorCreated()`          | Actor 被 spawn / editor 创建后             | Actor spawn path；construction 前                                                                                                                  |
| `OnConstruction()`            | Actor construction script 阶段             | Actor-only；对应 Blueprint Construction Script；作用于 Actor instance                                                                              |
| `PreInitializeComponents()`   | Actor components 初始化前                  | Actor-only；通常晚于 construction                                                                                                                  |
| `PostInitializeComponents()`  | Actor components 初始化后                  | Actor-only；进入 `BeginPlay()` 前                                                                                                                  |

补充：

- 每个 `UClass` 都有 Class Default Object（`CDO`），作为 class 级默认对象和普通 instance 的默认值来源
- native CDO 主要来自 C++ constructor / default subobject；Blueprint CDO 还会叠加 Blueprint asset 中保存的 Class Defaults 和 component templates
- `CreateDefaultSubobject<T>(Name)` 只应在 constructor 中用；它创建的是带 Outer 关系的 default subobject template

#### 生命周期

- 构造期逻辑优先看上一节；不要把 constructor 当作 gameplay init
- `BeginPlay()`: 进入 gameplay，适合依赖 `World`、其他 Actor、Editor 配置值的初始化
- `Tick()`: 每帧更新，需显式开启
- `EndPlay()` / `Destroyed()`: Actor 退出世界或销毁

#### GC

- UE GC 是 mark-sweep，不是普通引用计数
- 从 Root Set 出发，沿 `UPROPERTY` / reflected container 等可见引用标记对象
- 未被标记的 `UObject` 会在 GC 时回收，GC 通常在帧间执行
- `AddToRoot()` / `RemoveFromRoot()` 可手动加入 Root Set，慎用
- `AActor` 的生命周期主要由 `UWorld` 管理，销毁用 `Destroy()`
- `UActorComponent` 生命周期通常跟随 owner `AActor`

#### 引用规则

| 类型                              | 用途                                            |
| --------------------------------- | ----------------------------------------------- |
| `TObjectPtr<T>`                   | UE5 推荐的 `UObject` 成员引用，配合 `UPROPERTY` |
| `TWeakObjectPtr<T>`               | 弱引用 `UObject`，对象销毁后可检测失效          |
| `TSoftObjectPtr<T>`               | 软引用资产，记录路径，可延迟加载                |
| `TSubclassOf<T>`                  | 限制 Class 类型必须继承自 `T`                   |
| `TStrongObjectPtr<T>`             | 非 `UPROPERTY` 场景下强持有 `UObject`           |
| `TSharedPtr<T>` / `TSharedRef<T>` | 管理非 `UObject` 的原生 C++ 对象                |

#### Outer vs Owner

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

#### Replication

- Replication 只从 server 同步到 client
- `UPROPERTY(Replicated)`: 同步属性，需在 `GetLifetimeReplicatedProps()` 注册
- `ReplicatedUsing=OnRep_X`: client 收到属性更新后回调
- RPC 本质是通过网络调用 `UFUNCTION`
- `Server` RPC: client 请求 server 执行，常用于输入意图
- `Client` RPC: server 指定某个 owning client 执行
- `NetMulticast` RPC: server 广播到相关 client
- `GameMode` 不复制；跨端状态放 `GameState` / `PlayerState`

### Blueprint

Blueprint 本质是资产化的 `UClass` 扩展：继承 native class 或另一个 Blueprint class，保存默认属性、component tree、graph 和 generated class。

| 关注点      | 要点                                                                          |
| ----------- | ----------------------------------------------------------------------------- |
| Class / CDO | Blueprint 编译后生成 `UBlueprintGeneratedClass`，默认值落在该 class 的 `CDO`  |
| Graph       | Event Graph / Function Graph 会编译成 Blueprint VM 执行的数据 / glue          |
| 变量        | Blueprint 变量本质是 reflected property；默认值序列化在 asset / CDO 中        |
| 组件        | Blueprint component tree 叠加在 C++ default subobject 之上                    |
| 资产引用    | object reference 通常是 hard reference；soft reference 保存 object path       |
| 运行时实例  | spawn / load 时从 class default / archetype 复制默认值，再进入 Actor 生命周期 |

易混点：

- 修改 Blueprint 默认值是在改 class default，不是改某个已运行 instance
- Construction Script 适合编辑期装配和预览，例如根据参数调整 mesh、collision、材质, 它可能在 Editor 中频繁执行，不适合放昂贵逻辑或持久副作用
- Blueprint 里引用具体资产 / class 容易形成 hard reference，影响加载链和 cook 依赖
- Blueprint class 路径常见形式是 `/Game/BP_Hero.BP_Hero_C`，不是 `.uasset` 文件路径
- 性能敏感、复杂循环、底层系统优先放 C++，Blueprint 适合组合、配置和高层 gameplay glue

### Assets

UE asset 不是直接用磁盘路径访问，而是通过 package name / object path 访问。
物理文件先被注册到 mount point，再转换成 long package name，最后定位到 package 内的 `UObject`。

```text
Content/Characters/Hero.uasset
  -> /Game/Characters/Hero
  -> /Game/Characters/Hero.Hero
```

#### 虚拟文件系统

- 这里的 virtual file system 主要指 asset package namespace / mount point，不是普通磁盘文件 API
- UE 的资产路径是虚拟 package path，不是操作系统文件路径
- `Content/` 通常挂载到 `/Game/`
- Engine content 通常挂载到 `/Engine/`
- Plugin content 通常挂载到 `/<PluginName>/`
- C++ 反射类型通常挂载到 `/Script/<ModuleName>.<TypeName>`
- 自定义挂载点可通过 package 系统注册，核心是把物理目录映射到虚拟 package root
- Cook / Package 后物理文件可能进入 `.pak` / `.ucas`，但虚拟 package path 基本保持稳定
- 只有已注册 mount point 的路径才能在 package 系统中解析

常见映射：

| 物理来源                          | 虚拟根路径          | 示例                                      |
| --------------------------------- | ------------------- | ----------------------------------------- |
| `Content/`                        | `/Game/`            | `/Game/Characters/Hero`                   |
| `Engine/Content/`                 | `/Engine/`          | `/Engine/EngineMaterials/DefaultMaterial` |
| `Plugins/MyPlugin/Content/`       | `/MyPlugin/`        | `/MyPlugin/Items/Sword`                   |
| C++ reflected type in `Engine`    | `/Script/Engine`    | `/Script/Engine.Actor`                    |
| C++ reflected type in game module | `/Script/MyProject` | `/Script/MyProject.MyActor`               |

#### Package / Object 路径

| 概念                      | 形式                                                          | 含义                                       |
| ------------------------- | ------------------------------------------------------------- | ------------------------------------------ |
| Physical File Path        | `Content/Characters/Hero.uasset` / `Content/Levels/Main.umap` | 磁盘文件                                   |
| Mount Point               | `/Game/`                                                      | 虚拟根，把物理目录映射进 package namespace |
| Long Package Name         | `/Game/Characters/Hero`                                       | package 名，不带扩展名、不含对象名         |
| Top-level Object Path     | `/Game/Characters/Hero.Hero`                                  | package 内 top-level object                |
| Subobject Path            | `/Game/Maps/Main.Main:PersistentLevel.X`                      | top-level object 下的 subobject            |
| Blueprint Generated Class | `/Game/BP_Hero.BP_Hero_C`                                     | Blueprint asset 生成的 `UClass`            |
| Native Class Path         | `/Script/Engine.Actor`                                        | C++ 反射类型，不对应 `Content/` 文件       |

易混点：

- `/Game/Foo/Bar` 是 package name，不是文件夹路径；磁盘上通常对应 `Content/Foo/Bar.uasset`
- `/Game/Foo/Bar.Bar` 才是 object path，`.` 后面是 package 内对象名
- asset 文件名和 top-level object 名通常相同，但概念上不是一回事
- `.umap` 也是 package，只是主要保存 `UWorld` / `ULevel` 相关对象
- `/Script/Module.Type` 是 native reflected type path，不表示 `Source/` 被挂载

#### 名称 API

| API / 类型           | 结果示例                                  | 备注                                       |
| -------------------- | ----------------------------------------- | ------------------------------------------ |
| `GetName()`          | `Hero`                                    | 对象自身名字，不含 package / outer         |
| `GetPathName()`      | `/Game/Characters/Hero.Hero`              | object path；非 top-level object 会带 `:`  |
| `GetFullName()`      | `SkeletalMesh /Game/Characters/Hero.Hero` | class name + object path                   |
| `GetOuter()`         | `UPackage` / outer object                 | asset 的 outer 通常是 `UPackage`           |
| `FPackageName`       | package path conversion                   | filename / long package name / mount point |
| `FTopLevelAssetPath` | `/Game/Characters/Hero.Hero`              | top-level asset，不含 subobject path       |
| `FSoftObjectPath`    | `/Game/Maps/Main.Main:SubPath`            | soft reference，可包含 subobject path      |

#### 引用方式

- Hard Reference: 直接引用资产；加载当前对象时可能连带加载依赖
- Soft Reference: 只记录 object path，需要时手动 / 异步加载
- `TSoftObjectPtr<T>`: soft object reference，适合大资源、可选资源、跨关卡资源
- `TSoftClassPtr<T>`: soft class reference，常用于 Blueprint class
- `ConstructorHelpers::FObjectFinder`: constructor 中硬查找默认资产，只适合 native 默认绑定
- `TSoftObjectPtr::LoadSynchronous()`: 同步加载，简单但可能卡顿
- `StreamableManager`: 异步加载 soft reference

Editor 中选择资产不必然等于 hard reference，关键看属性类型：

- `UObject*` / `TObjectPtr<T>` / Blueprint object reference：通常形成 hard reference，引用者加载时目标资产也会进入加载链
- `TSoftObjectPtr<T>` / `TSoftClassPtr<T>` / `FSoftObjectPath`：保存 object path，通常形成 soft reference，需要时再加载
- Component 默认 mesh / material、Blueprint 默认值里选具体资产，常见是 hard reference
- 大型可选资源优先 soft reference；用 Reference Viewer / Size Map 查意外依赖

## Gameplay Framework

### Runtime 层级

Gameplay runtime 的主线是：`GameInstance` 保存跨关卡会话状态，`World` 承载当前运行世界，`GameMode` 定义当前世界规则，`PlayerController` 表示玩家控制入口，`Pawn` / `Character` 是当前被控制的身体。

```text
UEngine
  -> FWorldContext
    -> UGameInstance
      -> ULocalPlayer
        -> APlayerController references
          -> PlayerState reference
          -> Pawn / Character reference
    -> UWorld
      -> GameMode reference
      -> GameState reference
        -> PlayerArray[]
          -> APlayerState
      -> Levels[]
        -> ULevel
          -> ALevelScriptActor
          -> Actors[]
            -> AGameMode / AGameState / APlayerState
            -> APlayerController
            -> APawn / ACharacter
            -> other Actors / Components
```

核心对象：

| 类别               | 对象 / 类型              | 生命周期 / 所属                       | 主要职责 / 要点                                        |
| ------------------ | ------------------------ | ------------------------------------- | ------------------------------------------------------ |
| Engine context     | `UEngine`                | engine 全局                           | 管理 world context、viewport、engine 级服务            |
| Engine context     | `FWorldContext`          | engine 内部上下文                     | 追踪 `UWorld` / `UGameInstance` / travel / PIE context |
| Session            | `UGameInstance`          | 游戏会话级，通常跨 `OpenLevel()` 存活 | 跨关卡状态、save slot、全局配置、session data          |
| Local player       | `ULocalPlayer`           | 本地玩家槽位，split screen 时可有多个 | 本地输入、viewport、引用本地 `PlayerController`        |
| World              | `UWorld`                 | 当前运行世界实例，不等于 `.umap` 文件 | levels、Actor、timer、physics、navigation、trace       |
| World              | `ULevel`                 | `World` 中的关卡数据块                | 承载 `Actors[]`                                        |
| World              | `AWorldSettings`         | level 级 Actor                        | level 配置                                             |
| World              | `ALevelScriptActor`      | 属于某个 `ULevel`                     | Level Blueprint 的运行时实例，本关专用事件编排         |
| Gameplay framework | `AGameMode`              | 当前 `World` 规则对象，server-only    | 规则决策、spawn、胜负、match flow                      |
| Gameplay framework | `AGameSession`           | server-only                           | 登录、会话、匹配相关                                   |
| Gameplay framework | `AGameState`             | 当前 `World` 公共状态，可复制         | 波数、比分、倒计时、目标状态、`PlayerArray`            |
| Gameplay framework | `APlayerState`           | 单个玩家的公共状态，通常跨 pawn 存活  | 玩家名、分数、队伍、match 内玩家数据                   |
| Gameplay framework | `APlayerController`      | `World` 中的 Actor，本地玩家也会引用  | 输入、视角入口、possession、UI input mode              |
| Gameplay framework | `APawn` / `ACharacter`   | 可被 `Controller` possess 的 Actor    | 当前被控制的身体；`Character` 带人形 movement          |
| World entity       | `AActor`                 | `ULevel.Actors[]` 中的世界实体        | transform、spawn / destroy、component owner            |
| World entity       | `UActorComponent`        | Actor 的子对象                        | 行为模块，无 transform                                 |
| World entity       | `USceneComponent`        | Actor 的空间 component                | 带 transform，可组成 attach 层级                       |
| Subsystem          | `UEngineSubsystem`       | engine 级                             | 普通 gameplay 项目较少用                               |
| Subsystem          | `UGameInstanceSubsystem` | 跟随 `GameInstance`，跨关卡           | save slot、settings、全局配置                          |
| Subsystem          | `ULocalPlayerSubsystem`  | 每个 `ULocalPlayer` 一份              | input mapping、本地 UI 偏好                            |
| Subsystem          | `UWorldSubsystem`        | 跟随当前 `World`，切关卡重建          | spawn director、weather、objective registry            |

`.umap` 是磁盘资产；`UWorld` 是运行时世界对象；`ULevel` 是 `World` 内部的关卡数据块。
简单地图通常只有 `PersistentLevel`，streaming / World Partition 会让一个 `World` 包含多个 level 数据块。

### 运行时关系

运行时关系先分清三条主线：**容器 / 引用**、**控制 / 身体**、**规则 / 状态**。

**容器 / 引用关系**

| 生命周期域      | 功能归属 / 扩展方式                                                                                                                                                         |
| --------------- | --------------------------------------------------------------------------------------------------------------------------------------------------------------------------- |
| `UEngine`       | engine 全局上下文；管理 editor、PIE、preview、game 等多个 world context                                                                                                     |
| `UGameInstance` | 跨关卡 session 逻辑与数据；复杂全局服务优先拆到 `UGameInstanceSubsystem`                                                                                                    |
| `UWorld`        | 当前 world / 关卡运行域；管理关卡内通常可复用的逻辑与数据，常用 `UWorldSubsystem` 扩展                                                                                      |
| `ULevel`        | `World` 中的 Actor 数据块；`AGameMode`、`AGameState`、`PlayerController`、`Pawn` 等本质都在 level actor 集合中；本关专用逻辑可用 `ALevelScriptActor` / Level Blueprint 扩展 |

**控制 / 身体关系**

| 生命周期域             | 关系 / 扩展点                                           | 数据 / 逻辑归属                                                                                                |
| ---------------------- | ------------------------------------------------------- | -------------------------------------------------------------------------------------------------------------- |
| `ULocalPlayer`         | references `APlayerController`                          | 本地玩家槽位，通常跨关卡；本地玩家相关逻辑与数据可用 `ULocalPlayerSubsystem` 扩展                              |
| `APlayerController`    | possesses `Pawn` / `Character`                          | 当前 world 中的控制入口；通常跨 Pawn 生命周期，管理输入、视角、UI、possession                                  |
| `APlayerState`         | referenced by `PlayerController`                        | match/world 内玩家公共状态；通常跨 Pawn 生命周期，保存分数、名字、队伍等可复制数据                             |
| `APawn` / `ACharacter` | possessed by `Controller`; extended by `ActorComponent` | 当前被控制的身体；管理角色生命周期内的移动、战斗、血量、动画等逻辑与数据；可用 `ActorComponent` 拆分可复用能力 |

**GameMode / Match 关系**

| 生命周期域 / 对象   | 功能归属 / 扩展方式                                                                                                                                                                 |
| ------------------- | ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------- |
| `AGameMode`         | 当前 world 的 server-only 规则入口；管理登录、spawn、重生、胜负、match flow 等规则决策                                                                                              |
| `AGameSession`      | server-only 会话对象；管理登录许可、session、匹配相关逻辑，通常由 `GameMode` 使用                                                                                                   |
| `AGameState`        | 当前 world 的可复制公共状态；包含 `PlayerArray[]` 用于观察所有玩家的 `APlayerState`；client / UI 应读这里，而不是直接依赖 `GameMode`                                                |
| `APlayerState`      | 单个玩家的可复制公共状态；保存玩家名、分数、队伍等通常跨 Pawn 生命周期的数据                                                                                                        |
| `APlayerController` | 单个玩家在当前 world 的控制入口；存在于 server 和 owning client，不在所有 client 上作为公共状态存在；连接 `GameMode` spawn / restart 结果与当前 Pawn；公共玩家数据放 `APlayerState` |

单机游戏本质上也在本地运行 authority 逻辑；仍建议按 server-only / replicated state 的边界组织代码，避免以后扩展和 UI 耦合出问题。

### 玩家启动 / Spawn

典型玩家启动流程：

```text
OpenLevel / enter World
  -> create UWorld and load Level Actors
  -> create GameMode / GameState
  -> create PlayerController
  -> GameMode choose PlayerStart
  -> GameMode spawn DefaultPawnClass
  -> PlayerController possess Pawn
  -> BeginPlay gameplay
```

`GameMode` 默认会尝试为玩家生成 `DefaultPawnClass` 并让 `PlayerController` possess 它；设置好 `DefaultPawnClass` 后，通常不需要手动 `SpawnActor()` 玩家角色。

常见相关函数 / API：

| API / 函数                               | 作用                              |
| ---------------------------------------- | --------------------------------- |
| `InitGame()`                             | 初始化当前 `GameMode`             |
| `PreLogin()` / `Login()` / `PostLogin()` | 玩家连接 / 登录流程               |
| `HandleStartingNewPlayer()`              | 处理新玩家开始游戏                |
| `RestartPlayer()`                        | 生成 / 重生玩家入口               |
| `ChoosePlayerStart()`                    | 选择出生点                        |
| `SpawnDefaultPawnFor()`                  | 生成默认 Pawn                     |
| `GetDefaultPawnClassForController()`     | 决定某个 Controller 使用哪个 Pawn |

`PlayerStart` 是出生点候选；多个 `PlayerStart` 通常只是多个候选点，不会天然生成多个玩家角色。
场景中出现两个玩家角色时，优先检查：

- 是否手动放了一个 `Pawn` / `Character` 并设置 `Auto Possess Player = Player 0`
- 当前 `GameMode.DefaultPawnClass` 是否又自动生成了一个 Pawn
- 当前 map 的 `World Settings -> GameMode Override` 是否覆盖了项目默认 `GameMode`

### Actor / Component

Actor / Component 的主线是“实体 + 能力组合”，不要只靠继承堆 gameplay 类型。

| 层级              | 核心含义 / 职责                                                                             | 典型用途 / 边界                                  |
| ----------------- | ------------------------------------------------------------------------------------------- | ------------------------------------------------ |
| `AActor`          | `World` 中可 spawn / destroy 的实体；负责 world identity、transform、组件组合、对外高层行为 | 门、敌人、拾取物、投射物；不要承载所有可复用规则 |
| `UActorComponent` | 挂在 Actor 上的行为模块，无 transform；负责可复用能力、局部状态、public API、事件通知       | health、inventory、interaction、combat           |
| `USceneComponent` | 带 transform，可 attach，参与空间层级                                                       | mesh、camera、spring arm、collision shape        |
| Blueprint child   | 继承 C++ Actor / Component 后做装配和调参                                                   | mesh、sound、VFX、默认参数、少量 glue            |

- `AActor` 的 transform 通常来自 `RootComponent`
- `USceneComponent` 继承自 `UActorComponent`，但额外参与空间层级
- 没有空间意义的能力优先用 `UActorComponent`，例如 `HealthComponent`
- 需要位置 / 旋转 / attach 的对象用 `USceneComponent`，例如 mesh、camera、collision shape
- `ActorComponent` 不应写成“小 Actor”；它管理自己的能力和状态，owner 决定这些能力在当前实体上的意义
- 多个 Actor 复制粘贴同一组变量和函数时，优先拆 component；逻辑强依赖某个 Actor 独特结构时，留在 Actor 更清楚

典型组合：

```text
ACharacter
  -> CapsuleComponent // RootComponent
    -> Mesh
    -> SpringArm
      -> Camera
  -> HealthComponent
  -> CombatComponent
```

常见流程：

```text
外部调用 Actor 高层函数
  -> Actor 转发给 Component
  -> Component 修改自己的状态
  -> Component broadcast delegate
  -> Actor / UI / GameMode / Blueprint 监听后响应
```

### Subsystem

`Subsystem` 是自动挂在某个生命周期域上的 service，适合替代“每张图都要手放的 manager actor”。

| 需求                                 | 常见归属                | 原因                         |
| ------------------------------------ | ----------------------- | ---------------------------- |
| save slot、settings、跨关卡配置      | `GameInstanceSubsystem` | 跟随 `GameInstance`，跨关卡  |
| 当前关卡刷怪、天气、目标注册         | `WorldSubsystem`        | 跟随当前 `World`，切关卡重建 |
| Enhanced Input mapping、本地 UI 偏好 | `LocalPlayerSubsystem`  | 每个本地玩家一份             |
| 某张地图一次性过场 / 机关编排        | `ALevelScriptActor`     | 强绑定当前 level             |
| 某个 Actor 的能力                    | `ActorComponent`        | 依附实体，可复用             |

- `WorldSubsystem` 不应硬引用某张地图里的具体 Actor；用注册、tag、interface 或关卡脚本注入更清晰
- `LevelScriptActor` 不适合承载通用 gameplay 系统，避免多地图复制粘贴
- `GameInstance` 可保存跨关卡状态，但复杂项目更推荐拆到 `GameInstanceSubsystem`
- save / load 流程通常放 `GameInstanceSubsystem`；Actor / Component 只提供 capture / apply 自身状态的接口
- 跨系统运行时消息可放在 subsystem 中集中分发；局部对象状态变化优先用 delegate

### Gameplay Communication

`direct call`、`interface`、`delegate`、`message` 都是常见通信方式；UE 中主要注意 Blueprint、反射和对象生命周期带来的边界。

| 方式                         | UE 中常见形态                                  | 备注                                      |
| ---------------------------- | ---------------------------------------------- | ----------------------------------------- |
| direct call                  | C++ 指针调用 / Blueprint object reference      | 关系明确时最清晰                          |
| interface                    | `UINTERFACE` / Blueprint Interface             | C++ 调 BP interface 优先用 `Execute_X()`  |
| delegate                     | dynamic multicast delegate / Event Dispatcher  | UI / 临时对象注意绑定和解绑               |
| message                      | `GameplayMessageSubsystem`                     | 跨系统广播事实，不替代 replication / save |

`GameplayMessageSubsystem` 是 gameplay message 方案：通常用 `GameplayTag` 做 channel，用 `USTRUCT` 做 message body。

```text
Channel: Event.Item.PickedUp
Message: FItemPickedUpMessage { Picker, ItemId, Count }
```

适合 `Item.PickedUp`、`Quest.ObjectiveCompleted`、`Enemy.Spawned` 这类跨系统事实广播。
发送者不关心接收者；接收者按 channel 订阅。
局部明确关系不要强行上 message bus，否则控制流难追。

### Data Driven Gameplay

数据驱动的主线是把静态内容、运行时状态、行为执行、持久化快照分开。

| 层级                | 含义                       | 例子                                      |
| ------------------- | -------------------------- | ----------------------------------------- |
| Definition / Config | 静态配置，描述“这是什么”   | `UItemDefinition`、DataTable row、skill config |
| Runtime State       | 运行时结果，描述“现在怎样” | `CurrentHealth`、cooldown、`ItemId + Count` |
| Actor / Component   | 行为执行                   | `TryPickup()`、`ApplyDamage()`、`CanActivateSkill()` |
| Save Data           | 恢复游戏所需的稳定快照     | `LevelId`、`SaveId`、`ItemId + Count`     |

规则：

- C++ 定义数据结构和解释规则；`DataTable` / `DataAsset` 填具体内容；Blueprint 负责装配、默认值和表现
- `DataTable` / `DataAsset` 不保存 runtime state；`SaveGame` 保存稳定 ID 和 value，不保存 `AActor*`
- `DisplayName` 不是 ID；`ItemId`、`SkillId`、`EnemyId`、`LevelId`、`SaveId` 应保持稳定
- 背包条目保存 `ItemId + Count`；显示图标 / mesh / 文本时再通过 ID 查 definition

静态配置资产选择：

| 类型                | 适合场景                      | 要点                                          |
| ------------------- | ----------------------------- | --------------------------------------------- |
| `DataTable`         | 大量结构一致的表格数据        | row struct 继承 `FTableRowBase`；`RowName` 是 key |
| `UDataAsset`        | 一个资产表达一份复杂配置      | weapon、skill、loot table、enemy config       |
| `UPrimaryDataAsset` | 需要 `Asset Manager` 管理的主内容资产 | item、enemy、level、ability 等按 type / id 加载 |

`TSoftObjectPtr` / `TSoftClassPtr` 是 soft reference：保存 object path，不立即加载具体资产。
`Asset Manager` 管 `Primary Asset` 的发现、按 `PrimaryAssetId` 加载、卸载、cook / chunk / audit。
Project Settings 中配置 `Primary Asset Types to Scan` 后，启动时通常只是扫描 / 注册 `PrimaryAssetId -> AssetData`，不会自动把资产对象加载进内存；需要时显式 `LoadPrimaryAsset()`。

```text
字段引用具体资源 -> TSoftObjectPtr / TSoftClassPtr
管理一类游戏内容 -> UPrimaryDataAsset + Asset Manager
常见组合 -> Asset Manager 管 definition，definition 内部用 soft reference 引用 icon / mesh / sound
```

### Gameplay Tags

`Gameplay Tags` 用层级化 `FName` 表达分类、状态、条件和效果关系。

```text
Damage.Fire
State.Stunned
Item.Weapon.Sword
Enemy.Type.Undead
Faction.Enemy
```

| 需求                     | 更适合                                |
| ------------------------ | ------------------------------------- |
| 互斥流程状态             | `enum`                                |
| 可叠加分类 / 状态 / 效果 | `Gameplay Tags`                       |
| 层级匹配和数据驱动条件   | `Gameplay Tags` / `FGameplayTagQuery` |
| 可调用能力               | interface                             |
| 唯一内容身份             | `ItemId` / `FPrimaryAssetId`          |

常用类型：

| 类型                    | 用途                                          |
| ----------------------- | --------------------------------------------- |
| `FGameplayTag`          | 单个 tag                                      |
| `FGameplayTagContainer` | 一组 tag，支持 `HasTag` / `HasAny` / `HasAll` |
| `FGameplayTagQuery`     | 复杂 AND / OR / NOT 条件                      |
| `UGameplayTagsManager`  | 查询已注册 tag，例如 `RequestGameplayTag()`   |

注册方式：

- Project Settings -> Gameplay Tags
- `Config/DefaultGameplayTags.ini`
- C++ native tags：`UE_DECLARE_GAMEPLAY_TAG_EXTERN` / `UE_DEFINE_GAMEPLAY_TAG`

`RequestGameplayTag(TEXT("Damage.Fire"))` 是请求已注册 tag，不是注册。
核心 C++ 规则频繁使用的 tag 优先用 native gameplay tags，避免散落字符串。

典型数据驱动字段：

```cpp
UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Tags")
FGameplayTagContainer AbilityTags;

UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Tags")
FGameplayTagContainer RequiredOwnerTags;

UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Tags")
FGameplayTagContainer BlockedOwnerTags;
```

常见坑：

- runtime tag 要有添加和移除时机，例如 `State.Stunned` 到时清除
- tag 表达分类 / 条件，不表达数值；伤害、持续时间、冷却仍用字段
- 不要把 tag 命名混成 `Fire`、`IsBoss`、`enemy_flying`；保持层级规范

### Input / Camera

Enhanced Input 的主线是：`LocalPlayer` 持有 mapping context，`PlayerController` / `Pawn` 绑定响应，`Pawn` 执行动作。

| 概念                                          | 作用                                              |
| --------------------------------------------- | ------------------------------------------------- |
| `UInputAction`                                | 输入意图和值，如 Move、Look、Jump、Interact       |
| `UInputMappingContext`                        | 按键 / 手柄输入到 `InputAction` 的映射集合        |
| `UEnhancedInputLocalPlayerSubsystem`          | 给某个 `ULocalPlayer` 添加 / 移除 mapping context |
| `SetupPlayerInputComponent()`                 | 当前 Pawn 被 possess 后绑定输入响应函数           |
| `FInputModeGameOnly` / `UIOnly` / `GameAndUI` | 控制输入路由到 game / UI 和焦点行为               |

常见边界：

| 逻辑                          | 常见归属                                    |
| ----------------------------- | ------------------------------------------- |
| 添加 / 切换 mapping context   | `PlayerController` / `LocalPlayerSubsystem` |
| UI input mode、鼠标显示、暂停 | `PlayerController`                          |
| 移动、跳跃、攻击执行          | `Pawn` / `Character` / component            |
| 重生后仍存在的控制入口        | `PlayerController`                          |

Camera 关系：

- `PlayerController` 持有 `ControlRotation`，表示玩家视角意图
- `CameraComponent` 常放在 `Pawn` / `Character` 上，但最终 view target 由 `PlayerController` 控制
- `SpringArm` 常用于第三人称相机距离、碰撞避让、lag smoothing、跟随 control rotation
- `SetViewTargetWithBlend()` 可切换到独立 camera actor，用于过场、死亡视角、观察目标

常见第三人称设置：

```text
Character bUseControllerRotationYaw = false
CharacterMovement bOrientRotationToMovement = true
SpringArm bUsePawnControlRotation = true
```

### Collision Query

Collision Query 是“问世界里有什么”，不同于 physics simulation 的真实物理响应。

| Query     | 含义                        | 典型用途                       |
| --------- | --------------------------- | ------------------------------ |
| LineTrace | 一条线从 start 到 end       | 射击、交互视线、点击、视线检测 |
| Sweep     | 一个形状从 start 移动到 end | 近战轨迹、粗射线、高速命中检测 |
| Overlap   | 一个形状放在某位置查重叠    | 爆炸范围、触发区、拾取范围     |

- Sweep 有运动路径，适合“移动中的形状检测”；Overlap 只有当前范围，适合“范围内有什么”
- `Single` 返回一个主要命中；`Multi` 返回多个命中 / overlap 结果
- `Trace Channel` 是查询使用的问题类型，如 `Visibility`、`Camera`、自定义 `Interact` / `WeaponTrace`
- `Object Channel` 是目标对象类型，如 `Pawn`、`WorldStatic`、`PhysicsBody`
- 命中结果由 `Collision Enabled`、object type、channel、response、query params、shape 共同决定

常见 channel：

| Channel       | 用途                       |
| ------------- | -------------------------- |
| `Visibility`  | 通用视线 / 点击 / 简单交互 |
| `Camera`      | 相机阻挡、spring arm 避障  |
| `Interact`    | 自定义玩家交互检测         |
| `WeaponTrace` | 自定义武器 / 近战命中检测  |
| `AISight`     | 自定义 AI 视线遮挡         |

排查 trace 打不到对象时优先检查：debug draw、start/end、channel、目标 collision enabled、目标 response、ignored actors、目标组件是否有 collision。

### Tick / Timer / Async

UE gameplay 更新优先考虑事件驱动，其次 timer，最后才是 tick。

| 机制                         | 用途                                         | 注意                                    |
| ---------------------------- | -------------------------------------------- | --------------------------------------- |
| `Tick()` / `TickComponent()` | 每帧 gameplay 更新，如相机平滑、追踪、插值   | 不要每帧全局搜索 / 低频检查             |
| `FTimerManager`              | 延迟 / 周期执行，如 cooldown、回血、延迟开门 | 依赖 `World`，不要在 constructor 用     |
| Latent Action                | Blueprint 流程等待，如 `Delay`               | 不阻塞线程，依赖 world context          |
| Async Task                   | 后台计算 / IO / 数据解析                     | 修改 UObject / Actor 前回到 game thread |
| Tick Group                   | 控制 tick 相对 physics / camera 的顺序       | 相机抖动、物理后读位置时检查            |

常见判断：

- 每帧连续变化：`Tick`
- 固定时间后 / 周期触发：`Timer`
- 蓝图流程等待：latent action
- 耗时纯计算 / IO：async，结果回 game thread 应用
- 不确定线程安全时，默认认为 `UObject` / `Actor` 操作必须在 game thread

常见边界：

| 状态 / 逻辑             | 常见归属                                    |
| ----------------------- | ------------------------------------------- |
| save slot、跨关卡选择   | `GameInstance` / `GameInstanceSubsystem`    |
| 当前关卡规则、胜负判断  | `GameMode`                                  |
| 当前关卡可观察全局状态  | `GameState`                                 |
| 玩家输入、视角、UI 模式 | `PlayerController` / `LocalPlayerSubsystem` |
| 当前可死亡 / 可替换身体 | `Pawn` / `Character`                        |
| 玩家分数、名字、队伍    | `PlayerState`                               |

### Save Game

`USaveGame` 是存档数据容器；slot 是存档文件名 / 存档槽。存档保存“恢复游戏所需的最小稳定数据”，不是保存运行时对象。

| 关注点        | 要点                                                                 |
| ------------- | -------------------------------------------------------------------- |
| save object   | 继承 `USaveGame`，只放可序列化数据                                   |
| slot          | `AutoSave`、`Manual_01`、`Settings` 等；PC 单机 `UserIndex` 通常用 `0` |
| 归属          | `GameInstanceSubsystem` / `SaveSubsystem` 负责流程                   |
| world actor   | 用稳定 `SaveId` 恢复状态，不保存 actor 指针                          |
| versioning    | 一开始放 `SaveVersion`；加载老存档时做 migration                     |

常用 API：

| API                                 | 用途             |
| ----------------------------------- | ---------------- |
| `CreateSaveGameObject()`            | 创建 save object |
| `SaveGameToSlot(Save, SlotName, 0)` | 写入 slot        |
| `LoadGameFromSlot(SlotName, 0)`     | 读取 slot        |
| `DoesSaveGameExist(SlotName, 0)`    | 判断存档存在     |
| `DeleteGameInSlot(SlotName, 0)`     | 删除存档         |

保存内容：

| 保存                                 | 不保存                            |
| ------------------------------------ | --------------------------------- |
| `LevelId`、`CheckpointId`            | `UWorld*`、map runtime object     |
| `ItemId + Count`                     | item actor / mesh 指针            |
| `SkillId + Level`                    | skill runtime object              |
| `SaveId + opened/dead/current value` | `ADoor*`、`AEnemy*`               |
| `PlayerTransform`、`CurrentHealth`   | 临时 UI / effect / component 指针 |

- `Actor` / `Component` 只负责 capture / apply 自身状态；静态内容根据 ID 回查 `DataAsset` / `DataTable`
- 不要随意改 `ItemId`、`SaveId`、`LevelId`，否则旧存档找不到对象 / 配置
- settings 和 gameplay save 通常分 slot：`Settings` 跨存档，`AutoSave` / `Manual_01` 属于流程

### State Machine

状态机用于管理互斥流程状态；`Gameplay Tags` 用于可叠加分类和条件。

| 状态对象 | 示例状态                                                       | 常见归属                                 |
| -------- | -------------------------------------------------------------- | ---------------------------------------- |
| Enemy    | `Idle` / `Patrol` / `Chase` / `Attack` / `Dead`                | enemy actor / AI component / `StateTree` |
| Player   | `Normal` / `Attacking` / `Interacting` / `Dead`                | character / combat component             |
| GameFlow | `MainMenu` / `Loading` / `Playing` / `Paused` / `Won` / `Lost` | `GameMode` / `GameInstanceSubsystem`     |

最小状态机应集中切换入口：

```cpp
void SetState(EEnemyState NewState)
{
    if (CurrentState == NewState || !CanEnterState(NewState))
    {
        return;
    }

    const EEnemyState OldState = CurrentState;
    ExitState(OldState);
    CurrentState = NewState;
    EnterState(CurrentState);
    OnStateChanged.Broadcast(OldState, CurrentState);
}
```

要点：

- 不要到处直接改 `CurrentState`，否则 enter / exit 副作用会漏
- 死亡、暂停、胜负等高优先级状态要阻止输入、AI move、timer 或攻击流程继续
- `StateTree` 是 UE 的通用层级状态机方案；`UStateTreeComponent` 可挂在 Actor 上，`UStateTreeAIComponent` 常用于 AIController
- 简单三五个状态先用 `enum + SetState()`，理解 transition / enter / exit 后再上 `StateTree`

动画关联成熟做法：

```text
Gameplay state machine: 规则、能力、状态转移
AnimBP state machine: idle / walk / run / jump 等持续动画和 blend
Animation Montage: attack、roll、hit react、death 等一次性动作片段
AnimNotify: 命中窗口、开关碰撞、脚步声、VFX 等动画时机回调
```

典型攻击流程：

```text
Input Attack
  -> C++ TryAttack()
  -> SetState(Attacking)
  -> Play Montage
  -> AnimNotify_AttackWindowStart / Hit / End
  -> CombatComponent 做 trace / damage
  -> Montage ended / interrupted
  -> SetState(Normal)
```

不要让 AnimBP transition graph 成为核心 gameplay 规则唯一来源；动画提供表现和时机，最终规则仍回到 C++ / component。

## Editor

### Level Editor

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

### Node Graph

适用于 Blueprint Graph、Material Graph 等多数节点编辑器。

- `RMB + drag`: 平移 graph
- `Mouse Wheel`: 缩放
- `Home`: 缩放到选中节点
- `LMB drag` 空白处: 框选节点
- `Q`: 水平对齐选中节点
- `RMB click` 空白处: 打开节点搜索 / Action Menu
- `LMB drag` pin 到 pin: 连接
- `LMB drag` pin 到空白处: 按 pin 类型过滤创建节点
- `Alt + LMB click` pin: 断开该 pin 所有连接
- `Ctrl + LMB drag` pin: 移动该 pin 的所有连接
- `C`: 给选中节点添加 Comment
- `Ctrl + C/V/X`: 复制 / 粘贴 / 剪切节点
- `Ctrl + D`: Duplicate 节点
- `Delete`: 删除节点
- `Ctrl + Z/Y`: 撤销 / 重做
- `Ctrl + F`: 当前 Blueprint / graph 内查找
- `Ctrl + Shift + F`: Find in Blueprints
- `Ctrl + S`: 保存
- `F7`: Compile Blueprint
- `F9`: Toggle Breakpoint（Blueprint）
- Blueprint 创建: `B + LMB` Branch, `S + LMB` Sequence, `D + LMB` Delay, `P + LMB` BeginPlay
- Blueprint 变量: `Ctrl + drag` 到 graph 为 Get, `Alt + drag` 到 graph 为 Set
- Material 创建: `1/2/3/4 + LMB` Constant, `S + LMB` ScalarParameter, `V + LMB` VectorParameter, `T + LMB` TextureSample, `M + LMB` Multiply, `L + LMB` Lerp, `A + LMB` Add

### Material

- `Material` 定义表面如何被渲染，本质是生成 shader 的 node graph
- 常用 PBR 输入：`Base Color`、`Metallic`、`Roughness`、`Normal`、`Emissive Color`、`Opacity` / `Opacity Mask`
- 各输入属性都可以由 texture、constant 或 node expression 计算得到；常量适合快速调参，纹理适合空间变化细节
- `Material Domain`: `Surface` 最常用；还有 `Post Process`、`Deferred Decal`、`UI` 等
- `Blend Mode`: `Opaque` 性能最好；`Masked` 用 alpha test；`Translucent` 最贵且有排序 / lighting 限制
- `Shading Model`: 决定光照模型，如 `Default Lit`、`Unlit`、`Subsurface`、`Clear Coat`
- `Texture Sample` 通常配合 `UV` / `TexCoord`；`Normal Map` 需要接到 `Normal`，注意 texture compression / sRGB
- `ScalarParameter` / `VectorParameter` / `TextureParameter` 可在 `Material Instance` 中覆盖，不改 graph
- `Material Instance` 复用父材质 shader，适合做同一套材质的颜色、贴图、强度变体
- `Dynamic Material Instance` 可在 runtime 改参数；常用于受击闪烁、溶解、血条、交互高亮
- `Static Switch Parameter` 会产生 shader permutation，适合开关功能，但过多会增加编译和包体成本
- `Material Parameter Collection` 是全局参数表，适合天气、时间、全局风向等跨多个材质共享的数据
- 性能重点看 shader instructions、texture sample 数、overdraw、translucency、复杂分支和过多 permutation
- PBR (Physically Based Rendering)

```txt
Diffuse =
    LightColor *
    BaseColor *
    (1 - Metallic);

Specular =
    LightColor *
    ReflectionStrength * // affected by Roughness and Specular
    ReflectionColor; // lerp(0.04, BaseColor, Metallic);

FinalColor =
    Diffuse +
    Specular +
    Emissive;
```

重要输入属性：

| Property                | 要点                                              |
| ----------------------- | ------------------------------------------------- |
| `Base Color`            | 表面基础颜色，不包含光照                          |
| `Metallic`              | 金属度，通常非金属 `0`、金属 `1`，少用中间值      |
| `Specular`              | 非金属高光强度，默认 `0.5`，多数情况不用改        |
| `Roughness`             | 粗糙度；`0` 镜面，`1` 漫反射，最常调的质感参数    |
| `Normal`                | 法线贴图输入，增加表面细节，不改变真实几何        |
| `Emissive Color`        | 自发光颜色，可用于发光物、UI、Unlit 效果          |
| `Opacity`               | 半透明透明度，仅 `Translucent` 等 blend mode 有效 |
| `Opacity Mask`          | 裁剪透明度，用于 `Masked`，常配合 foliage / hair  |
| `Ambient Occlusion`     | 局部环境遮蔽，增强缝隙暗部                        |
| `World Position Offset` | 顶点阶段偏移几何，常用于风吹草、波浪、简单变形    |
| `Refraction`            | 折射，常用于玻璃 / 水，通常依赖 translucent       |
| `Pixel Depth Offset`    | 像素深度偏移，可软化交界，但可能影响深度相关效果  |
| `Subsurface Color`      | 次表面散射颜色，需对应 `Shading Model`            |
| `Clear Coat`            | 额外清漆层强度，车漆、涂层材质常用                |
