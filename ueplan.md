# Unreal Engine Learning Roadmap

目标：学习完后，只要给定美术资产，就能独立制作一个完整可运行的单机游戏。

这份路线图偏向给后续 agent 使用：学习时不要只按 UE 子系统平铺推进，而要围绕“能交付 playable game”的能力链组织知识。

## 总目标

最终应具备以下能力：

- `gameplay loop`: 输入、角色、交互、战斗 / 解谜 / 收集、胜负条件
- `content integration`: 导入并配置 mesh、material、animation、audio、VFX、UI
- `runtime architecture`: 理解 `UObject` / `Actor` / `Component` / `Subsystem` / `GameInstance` / `GameMode`
- `data driven`: 使用 `DataTable`、`PrimaryDataAsset`、soft reference、配置表组织游戏内容
- `persistence`: save / load、settings、checkpoint
- `production polish`: camera、feedback、UI、sound、VFX、lighting、post-process
- `optimization`: profiling、LOD、collision、tick、asset loading、packaging

核心判断标准：不是“知道每个系统是什么”，而是知道这些系统在一个游戏项目里如何协作。

## 学习顺序

1. Runtime / `UObject` / Gameplay Framework
2. C++ / Blueprint Bridge / Gameplay 架构
3. Asset Pipeline
4. Animation / Material / Mesh / LOD
5. Lighting / Post Process / Environment
6. Physics / Collision / Movement
7. AI / Enemy Behavior / Navigation
8. UI / Audio / VFX
9. Profiling / Packaging / Polish

## 1. Runtime 架构与 Gameplay Framework

优先级最高，比渲染、物理、音频更早。

重点：

- `UObject`、`AActor`、`UActorComponent`、`USceneComponent`
- `UWorld`、`ULevel`、`UGameInstance`
- `AGameMode`、`AGameState`、`APlayerController`、`APawn`、`ACharacter`
- object lifecycle：constructor、`PostLoad()`、`BeginPlay()`、`Tick()`、`EndPlay()`
- `CDO`、default subobject、runtime object、asset object 的区别
- `UPROPERTY`、GC、hard reference、soft reference、async loading
- `Subsystem`：`GameInstanceSubsystem`、`WorldSubsystem`、`LocalPlayerSubsystem`
- Enhanced Input、camera、spawn、possession、collision query
- `FTimerManager`、latent action、async task、game thread、tick group

阶段产物：

- 一个 C++ gameplay prototype
- 包含玩家控制、交互物、状态管理、关卡切换、保存进度

易忘点：

- constructor 主要初始化 `CDO` 和 default subobject，不要依赖 `World`
- `GameMode` server-only，单机也应理解其职责边界
- `PlayerController` 不是角色本体，角色通常是 `Pawn` / `Character`
- `Outer`、`Owner`、attach parent 是三套不同关系

## 2. C++ / Blueprint Bridge 与 Gameplay 架构

这一层决定能不能把游戏做完整，而不是只做 demo。

重点：

- C++ / Blueprint bridge：
  - `UCLASS` / `USTRUCT` / `UENUM` / `UINTERFACE`
  - `UFUNCTION` / `UPROPERTY` metadata
  - `BlueprintCallable`、`BlueprintPure`、`BlueprintImplementableEvent`、`BlueprintNativeEvent`
  - `ExposeOnSpawn`、`EditDefaultsOnly`、`EditInstanceOnly`、`BlueprintReadOnly` / `BlueprintReadWrite`
  - Construction Script、Blueprint debugging、Find in Blueprints
  - Blueprint Interface、Event Dispatcher、delegate、soft class reference
- Actor vs Component 的职责拆分
- C++ 和 Blueprint 的边界：核心逻辑 C++，可调参数和表现 Blueprint
- `UDataAsset` / `UPrimaryDataAsset` / `DataTable`
- item、skill、enemy、level config 的数据建模
- `Gameplay Tags`
- event、delegate、interface、message-style communication
- save game：`USaveGame`、slot、versioning
- simple state machine：player state、enemy state、game flow state

阶段产物：

- 一个灰盒 gameplay vertical slice
- 使用 placeholder mesh / basic HUD text / debug feedback，不要求最终 UI、音效、特效
- 至少包含玩家、交互、一个规则目标、胜负条件、存档或 checkpoint

设计原则：

- 先拆数据，再写逻辑；能用配置表达的内容不要硬编码
- `ActorComponent` 适合拆可复用行为，避免 `Actor` 继承层级膨胀
- `Blueprint` 适合表现、装配、调参，不适合承载复杂核心规则
- save data 不应直接保存大量运行时对象引用
- 需要给 designer / future self 调的参数，用 metadata 明确暴露边界

## 3. Asset Pipeline 与资源管理

目标是：给一套美术资产后，能接入、整理、引用、打包，并在 packaged build 中正常运行。

重点：

- Static Mesh / Skeletal Mesh 导入流程
- FBX / glTF / texture 命名和目录规范
- collision setup：simple collision、complex collision、physics asset
- Skeleton、Animation Sequence、Animation Blueprint、BlendSpace、Montage
- texture 设置：sRGB、compression、normal map、mipmap、virtual texture
- material instance 参数化
- soft reference、asset streaming、Asset Manager
- `Primary Asset Type`、`Primary Asset Id`、`PrimaryAssetLabel`
- chunk、cook rule、asset audit、Reference Viewer
- redirector、Fix Up Redirectors
- cook / package 后资源路径和加载行为

阶段产物：

- 导入并配置一套角色、场景、道具、音效、VFX 资产
- 项目能在 packaged build 中正确加载资源
- 用 `Primary Asset` 管理一类可动态加载内容，例如 item、enemy、level

易忘点：

- DataTable / DataAsset 中引用大资产时优先考虑 soft reference
- texture 的 `sRGB` 和 compression 设置错误会直接影响材质结果
- Editor 下能加载，不代表 packaged build 中路径和引用一定正确
- 改名 / 移动资产后及时检查 redirector 和引用链

## 4. Animation、Materials 与 Rendering Performance

目标是把“程序上可玩”的游戏做成“看起来完整”。

重点：

- Static Mesh / Skeletal Mesh 渲染路径概念
- Material：PBR、shader permutation、material instance、dynamic material instance
- `Opaque` / `Masked` / `Translucent` / `Deferred Decal` / `UI` material 的成本和限制
- LOD、Nanite、HLOD、culling、draw call、instancing
- Animation：AnimBP、state machine、blend、slot、montage、root motion、IK
- Post Process Volume：exposure、bloom、color grading、DOF、motion blur、outline

阶段产物：

- 角色动画切换自然
- 材质可通过 instance 调参
- gameplay 事件能驱动 material / animation / VFX
- 基础性能不因材质、透明、粒子、LOD 明显崩坏

易忘点：

- `Static Switch Parameter` 会产生 shader permutation
- `Translucent` 通常昂贵，并且有排序和 lighting 限制
- Animation Montage 适合动作片段触发，AnimBP state machine 适合持续状态

## 5. Lighting 与环境

不要只学参数，要学场景制作决策。

重点：

- Static / Stationary / Movable light
- Lumen、shadow、reflection、ambient occlusion
- SkyAtmosphere、DirectionalLight、SkyLight、ExponentialHeightFog
- lightmap、baked lighting 的基本概念
- exposure、tone mapping、color grading
- indoor / outdoor / stylized / realistic 的常见配置
- lighting 对性能的影响

阶段产物：

- 能为一个关卡建立基础氛围
- 能判断并修正常见问题：过暗、过曝、发灰、噪点、阴影闪烁、反射异常、性能过高

易忘点：

- lighting、post-process、material 会共同决定最终画面，不要只改一个系统
- movable shadow、translucency、反射和全局光照通常是性能重点

## 6. Physics、Collision 与 Movement

围绕 gameplay 使用物理，不要一开始钻太底层。

重点：

- collision channel、object type、trace channel、profile
- overlap vs hit vs trace
- `CharacterMovementComponent`
- physics simulation、constraint、force、impulse
- physics asset、ragdoll 基础
- projectile、melee hit detection、interaction trace
- navigation mesh 与 AI movement 基础

阶段产物：

- 玩家移动、拾取、攻击命中、机关触发、弹射物、简单物理物件都能工作

易忘点：

- collision response 是双方共同决定的
- trace channel 和 object channel 的用途不同
- `CharacterMovementComponent` 不是普通 physics simulation

## 7. AI、Enemy Behavior 与 Navigation

目标是能做出基本可信的 enemy / NPC 行为，而不是只会移动到目标点。

重点：

- `AIController`、`Pawn` possession、team / target 关系
- NavMesh、`NavMeshBoundsVolume`、`UNavigationSystemV1`
- `MoveTo`、path following、acceptance radius、partial path
- `Behavior Tree`、`Blackboard`
- `EQS`、`AIPerception`、sight / hearing
- `StateTree`
- simple finite state machine 和 UE AI 工具的取舍
- patrol、chase、attack、retreat、search、return home

阶段产物：

- 一个 enemy prototype
- 能巡逻、感知玩家、追击、攻击、丢失目标后回到巡逻

易忘点：

- NavMesh 只解决可走路径，不解决行为决策
- `Behavior Tree` 适合较复杂 AI，简单敌人可先用 FSM / component
- 感知、攻击距离、碰撞、动画事件需要一起调

## 8. UI、Audio、VFX

这三个系统决定游戏完整度和反馈质量。

### UI

重点：

- UMG、Widget Blueprint、HUD、menu、settings、pause
- UI 和 gameplay 数据同步
- `CommonUI`
- widget lifecycle：`OnInitialized`、`Construct`、`Destruct`
- binding 性能、event-driven UI update
- input mode、focus、input routing、gamepad navigation
- DPI scaling、safe zone、resolution / fullscreen settings
- input rebinding
- health bar、inventory、dialogue、tooltip、UI animation

### Audio

重点：

- Sound Wave、Sound Cue / MetaSound
- attenuation、spatialization、mix、bus、volume class
- `SoundClass`、`SoundMix`、`Submix`
- concurrency、randomization、loop、fade
- MetaSound 参数
- gameplay event 触发音效
- UI sound、ambient、music state transition
- settings menu 控制 master / music / SFX volume，并持久化

### VFX

重点：

- Niagara System / Emitter
- spawn at location / attach to actor / parameter control
- hit effect、loop effect、trail、area effect
- material + Niagara 配合

阶段产物：

- 一个完整 feedback loop：
  - 按键有响应
  - 攻击有动画、声音、命中特效
  - UI 状态变化及时
  - 成功 / 失败 / 受击 / 拾取等事件都有反馈
- settings / pause / gamepad navigation / volume control 可用

## 9. Debugging、Profiling、Packaging

这是从 demo 到可交付游戏的分界线。

重点：

- PIE vs Standalone vs packaged build 差异
- log、breakpoint、Blueprint debugger、Visual Logger
- `stat unit`、`stat game`、`stat gpu`、Unreal Insights
- tick cost、GC、asset loading hitch、shader compile、overdraw
- config、scalability、input settings
- `Development` / `Shipping` build configuration
- `Project Settings`
- `DefaultEngine.ini` / `DefaultGame.ini` / config hierarchy
- cook、package、pak / io store、shipping build
- prerequisites、command line args、save path
- crash log、crash dump 和 save compatibility

阶段产物：

- 能打出一个别人机器上可运行的包
- 能定位常见运行时问题
- 能列出平台 packaging checklist

易忘点：

- Editor 正常不等于 packaged build 正常
- Standalone、Development packaged、Shipping packaged 都可能有行为差异
- 性能问题先 profiling，不要凭感觉优化
- `Tick`、动态加载、透明材质、粒子、shadow、animation graph 都可能成为瓶颈

## 最小完整项目目标

建议用一个 10-15 分钟的单机关卡验证学习成果。

必须包含：

- main menu
- settings
- pause menu
- level transition
- restart level
- return to menu
- respawn / checkpoint
- `GameInstance` 保存跨关卡 runtime state
- level streaming / World Partition / Level Instance 至少理解取舍
- player controller / character
- camera
- interaction
- 至少一种核心玩法：战斗、解谜、收集、潜行、平台跳跃任选其一
- enemy / obstacle / puzzle object
- win / lose condition
- save / load 或 checkpoint
- HUD
- animation
- material instance
- sound effect
- background music 或 ambient sound
- Niagara VFX
- lighting
- post-process
- packaged build

验收标准：

- 没有 editor-only 依赖
- packaged build 可运行
- 新开机器或 clean build 后资源引用正常
- gameplay loop 闭环
- 场景切换、重开、返回菜单流程闭环
- UI / Audio / VFX 有基本反馈
- 性能问题能通过 profiling 给出原因
- Development packaged 和 Shipping packaged 都至少验证一次

## Agent 辅助学习方式

后续让 agent 辅助学习时，优先按以下方式推进：

1. 每次只学习一个阶段或一个小主题
2. 先要求 agent 解释核心概念、边界条件、常见坑
3. 再要求 agent 给出 C++ / Blueprint 对应实践
4. 最后要求 agent 设计一个可验证的小练习
5. 学完后把结论整理回 `ue.md`

推荐 prompt：

```text
我正在按 ueplan.md 学习 UE。现在学习第 X 阶段的 Y 主题。
请按个人速查笔记风格讲解：
1. 核心概念
2. UE 中的关键类型 / API
3. 生命周期或工作流程
4. 常见坑
5. 一个最小实践练习
6. 验收方式 / 常见失败现象
7. packaged build 注意点（如果相关）
8. 适合整理进 ue.md 的笔记摘要
```

## 学习记录建议

每个主题学完后，在 `ue.md` 中追加：

- 关键结论
- 类型 / API
- 生命周期
- 易错点
- 与其他系统的关系
- 最小代码片段

不要把笔记写成完整教程；目标是未来快速搜索和回忆。
