# Unreal Engine Learning Roadmap

目标：理解并掌握 Unreal Engine 的基础概念、核心机制和主要子系统；最终在 AI agent + MCP 辅助下，能够判断设计、指导实现、诊断问题，并完成一个可运行的单机游戏。

AI agent / MCP 只改变操作方式，不改变学习目标：

- 必须掌握决定架构、正确性、性能和调试方向的知识
- 具体 API 签名、Editor 点击路径、重复配置和批量资产操作可以按需查询或交给 agent
- agent 生成的代码、Blueprint 和资产配置必须能理解其作用、归属、生命周期和风险
- 不要求熟练记忆所有操作，但必须知道 UE 为什么这样工作，以及系统之间如何协作

## 学习范围判断

每个主题按三种深度处理：

| 深度 | 判断标准 | 例子 |
| --- | --- | --- |
| 必须掌握 | 影响架构、ownership、lifecycle、正确性、性能或诊断 | `UObject`、Gameplay Framework、asset reference、collision、game thread |
| 理解并会选择 | 需要知道用途、流程、边界和取舍，不必记全 API | Behavior Tree vs StateTree、Lumen vs baked lighting、Asset Manager |
| 可交给 agent | 重复、机械、易验证且不改变核心设计 | Editor 点击、批量命名 / 导入、创建样板类、填写重复配置 |

判断原则：如果 agent 做错后，必须依靠该知识才能发现或修复，就不能跳过。

## 学习主线

学习顺序以依赖关系为主，不按 Editor 面板或 API 数量展开：

```text
Object Model / Runtime
  -> Gameplay Framework
  -> Gameplay Architecture / Data
  -> World / Asset / Loading
  -> Collision / Movement / Physics
  -> Animation / AI
  -> Rendering / Environment
  -> UI / Audio / VFX
  -> Debugging / Profiling / Packaging
```

每阶段遵循：

```text
主要结构与核心流程
  -> ownership / lifecycle / data flow
  -> 相邻方案的选择边界
  -> 常见错误与诊断
  -> 最小实验
  -> 整理进 ue.md
```

## 当前进度

`ue.md` 已覆盖以下内容：

- Project / Plugin / Target / Module / Package
- `UObject`、Reflection、CDO、构造、生命周期、GC、引用、`Outer` / `Owner`
- Blueprint bridge、asset path、hard / soft reference
- Gameplay Framework、spawn / possession、Actor / Component / Subsystem
- gameplay communication、data-driven、Gameplay Tags
- Enhanced Input、camera、collision query、Tick / Timer / Async
- Save Game、state machine
- Level Editor、node graph、Material 基础

这些内容不必重新泛读。后续通过 recall、画流程和最小实验加深；答不出边界或无法诊断时，再回到对应章节补缺。

## 1. Object Model 与 Runtime

这是 UE 其余系统的共同基础，必须形成稳定心智模型。

### 必须掌握

- 普通 C++ object、`UObject`、`AActor`、`UActorComponent`、asset object 的区别
- Reflection、Serialization、GC、CDO、default object system 如何协作
- constructor、`PostInitProperties()`、`PostLoad()`、`OnConstruction()`、`BeginPlay()`、`Tick()`、`EndPlay()` 的适用边界
- class、instance、archetype、CDO、default subobject
- `Outer`、Owner、attach parent、World context 是不同关系
- strong / weak / soft reference；引用对 GC、loading、cook 的影响
- game thread、Tick、Timer、latent action、async task 的基本边界
- Editor world、PIE world、game world 不是同一个运行环境

### 理解即可

- object flags、package flags、GC 内部 mark / sweep 细节
- engine loop 和 task graph 的底层实现
- UObject serialization 的高级定制入口

### 最小验收

- 给任意对象说明：谁创建、谁持有、属于哪个 World、何时可用、何时销毁
- 解释为什么 constructor 中不能依赖 runtime World
- 诊断一次 dangling reference、GC 丢失引用或错误 World context 问题

## 2. Gameplay Framework 与游戏启动流程

重点不是背 class hierarchy，而是掌握一次游戏从启动到玩家可控制角色的完整流程。

### 必须掌握

- Engine -> GameInstance -> World -> Level 的生命周期关系
- `GameMode`、`GameState`、`PlayerController`、`PlayerState`、`Pawn` / `Character` 的职责
- map load、player join、spawn、possess、input binding、camera view 的主流程
- `GameMode` 是规则和 spawn policy，不是通用全局状态容器
- `PlayerController` 表示控制者；`Pawn` 是可被替换、死亡和重生的受控实体
- Actor / Component / Subsystem / Level Blueprint 的选择边界
- travel / level transition 时哪些对象保留，哪些重建
- Enhanced Input 的 LocalPlayer、Mapping Context、Input Action 和 Pawn response 链路

### 理解并会选择

- `GameInstanceSubsystem`、`WorldSubsystem`、`LocalPlayerSubsystem`
- seamless travel 与 multiplayer-specific framework；单机阶段了解职责即可
- World Partition、level streaming、Level Instance 的用途差异

### 最小验收

- 画出启动到 `BeginPlay()`、玩家生成、possession、input 生效的流程
- 实现角色死亡 -> unpossess / destroy -> respawn -> repossess
- 实现 main menu -> game level -> pause -> restart -> return menu

## 3. C++ / Blueprint 边界与 Gameplay 架构

目标是能设计稳定接口和状态归属，而不是学习尽可能多的 Blueprint node。

### 必须掌握

- `UCLASS`、`USTRUCT`、`UENUM`、`UINTERFACE`、`UPROPERTY`、`UFUNCTION` 的语义
- `BlueprintCallable`、`BlueprintPure`、`BlueprintImplementableEvent`、`BlueprintNativeEvent`
- C++ 定义规则、状态和协议；Blueprint 负责装配、表现和调参
- direct call、interface、delegate / Event Dispatcher、message 的选择
- Actor composition、Component 复用与继承层级的取舍
- state machine 的 state、transition、enter / exit 和 interruption
- gameplay state 与 animation state 的区别
- gameplay event 的发出者、消费者、时序和解绑生命周期

### 理解并会选择

- Blueprint Function Library、Blueprint Macro Library
- Gameplay Message Subsystem
- plugin / module 作为功能边界
- GAS：普通单机项目不必早学；大量 ability、attribute、effect、prediction 需求出现时再学

### 可交给 agent

- 创建样板 `UCLASS` / Component / Interface 文件
- Blueprint node 的机械连接和变量暴露
- metadata 和 API 签名查询

### 最小验收

- 把一个功能拆成 C++ rule、Component state、Blueprint presentation
- 能说明每个变量的唯一 owner，以及谁可以修改它
- 避免 circular dependency、全局搜索对象和滥用 cast

## 4. Data-driven、Asset 与 Loading

目标是理解“代码、配置、运行时状态、资产、存档”如何分离。

### 必须掌握

- Definition / Config、runtime state、behavior、save snapshot 四层
- package、asset、object path、class path、mount point
- hard reference、soft reference、weak reference 的 loading 和生命周期差异
- sync / async loading，以及加载完成前对象不可用
- stable ID 与 display name / asset path 的区别
- `DataTable`、`DataAsset`、`PrimaryDataAsset` 的数据模型和适用边界
- `Gameplay Tags` 用于分类 / 状态 / 条件，不替代唯一 ID 和数值字段
- `USaveGame` 只保存稳定、可序列化的恢复数据
- Editor 能找到资产不代表 cook / packaged build 一定包含资产

### 理解并会选择

- Asset Registry、Asset Manager、`PrimaryAssetId`
- primary asset scan、load bundle、cook rule、chunk
- redirector、Reference Viewer、Size Map、Asset Audit
- Data Registry：需要跨来源统一查询数据时再深入

### 可交给 agent

- 按既定 schema 创建 DataAsset / DataTable row
- 批量检查 ID、路径、命名和字段范围
- Fix Up Redirectors、批量移动和重复配置，但操作前后需检查引用

### 最小验收

- 用 ID 查 definition，再用 soft reference 异步加载表现资产
- 保存并恢复玩家和 world actor 的最小稳定状态
- 在 packaged build 验证动态加载资产确实被 cook

## 5. Collision、Movement 与 Physics

先掌握 gameplay 所需的空间规则，再按项目需要深入 physics simulation。

### 必须掌握

- collision enabled、object type、trace channel、response、profile 的组合关系
- overlap、hit、line trace、sweep 的语义和适用场景
- query 与 physics simulation 的区别
- collision 通常属于 `PrimitiveComponent`，不是抽象 Actor 自身
- `CharacterMovementComponent` 是 character movement model，不是普通刚体 simulation
- frame-rate independence、delta time、velocity、acceleration 的基本关系
- projectile、melee、interaction、trigger 的常见检测方案及失败边界
- debug draw、collision visualization、hit result 的诊断方法

### 理解并会选择

- force、impulse、constraint、physical material、Physics Asset、ragdoll
- sub-stepping、continuous collision detection
- Chaos 的底层 solver 细节按项目需要学习

### 可交给 agent

- 按规则创建 collision profile
- 给 mesh 批量生成 simple collision
- 重复放置 trigger、constraint 和 debug fixture

### 最小验收

- 实现 interaction trace、melee sweep、projectile hit、overlap trigger
- 能系统排查 trace 打不到、overlap 不触发、hit 重复触发
- 解释高速物体穿透和 collision response 双方配置问题

## 6. Animation

目标是理解 gameplay、animation graph 和时间点事件如何协作。

### 必须掌握

- Skeleton、Skeletal Mesh、Animation Sequence、AnimInstance / AnimBP 的关系
- locomotion data -> state machine / BlendSpace -> final pose 的主流程
- AnimBP state machine 处理持续动画状态；Montage 处理一次性动作片段
- slot、section、blend in / out、Montage interruption
- AnimNotify / Notify State 只提供动画时间点，不应成为核心规则的唯一来源
- root motion vs in-place movement 的 ownership 差异
- gameplay state、movement state、animation state 需要分离但保持同步
- animation thread 与 gameplay thread 的基本边界；AnimBP 中避免任意访问不安全状态

### 理解并会选择

- IK、Control Rig、retargeting、motion warping
- sync group、layered blend、linked Anim Graph
- animation compression 和 update rate optimization

### 可交给 agent

- 创建基础 state machine / BlendSpace 和重复 transition
- 批量 retarget、设置 slot、添加规范化 Notify
- 查询具体 animation node 和 property

### 最小验收

- 完成 idle / walk / run / jump locomotion
- 完成 input -> gameplay rule -> Montage -> hit window -> recovery / interruption
- 能排查 montage 不播放、slot 不生效、root motion 冲突和状态卡死

## 7. AI 与 Navigation

目标是理解“感知、决策、移动、执行”四层，而不是只会拼 Behavior Tree node。

### 必须掌握

- `AIController` possession、Pawn、NavMesh、path following 的关系
- perception data 只是 observation，不等于 truth 或 decision
- target selection、state、transition、action execution 的归属
- patrol、detect、chase、attack、lose target、search、return 的完整状态流
- movement request 可能失败、被取消、部分可达或因目标销毁失效
- AI gameplay rule、animation、collision 和 damage 的协作

### 理解并会选择

- 简单 FSM：少量明确、互斥状态
- Behavior Tree：持续决策、条件组合和 task 编排
- StateTree：层级状态、transition 和 task 组合
- EQS：环境候选位置 / 对象的评分选择
- `AIPerception` sight / hearing 的更新和遗忘行为

### 可交给 agent

- 创建 Blackboard key、Behavior Tree / StateTree 基础结构
- 配置 patrol point 和测试场景
- 重复调参，但状态定义和 transition 规则必须自己掌握

### 最小验收

- enemy 完成 patrol -> detect -> chase -> attack -> lose -> return
- 覆盖 unreachable target、target destroyed、death interruption
- 使用 Visual Logger / debug state 解释 AI 为什么做出某个行为

## 8. Rendering、Material 与 Environment

不要求成为 rendering engineer，但必须掌握最终画面由哪些阶段和数据共同决定。

### 必须掌握

- mesh -> material -> lighting -> post-process -> final image 的主流程
- PBR 中 Base Color、Metallic、Roughness、Normal、Emissive 的含义
- Material、Material Instance、Dynamic Material Instance 的关系
- `Opaque`、`Masked`、`Translucent` 的渲染和成本差异
- texture 的 sRGB、compression、normal map、mipmap
- Static Mesh / Skeletal Mesh、LOD、Nanite、HLOD、culling、instancing
- light mobility、shadow、reflection、global illumination 的基本取舍
- exposure、tone mapping、color grading 为什么会改变材质观感
- CPU frame、GPU frame、draw call、shader cost、overdraw 的基本概念

### 理解并会选择

- Lumen vs baked lighting
- Forward vs Deferred rendering
- Virtual Shadow Maps、Virtual Textures
- shader permutation 和 PSO 对编译 / 运行的影响
- advanced shader、render pass、RDG 按项目需求深入

### 可交给 agent

- 创建 Material Instance、批量设置 texture import / LOD
- 搭建常规 SkyAtmosphere、fog、light actor
- 批量检查昂贵 translucent material 和缺失 LOD

### 最小验收

- 能从 mesh、material、texture、lighting、post-process 分层排查画面问题
- 做一个可参数化材质和一个 runtime feedback 材质
- 使用 GPU profiling 识别一次 overdraw、shadow 或 material bottleneck

## 9. UI、Audio 与 VFX

这些是独立子系统，但应通过 gameplay event 消费状态，不反向拥有核心规则。

### UI 必须掌握

- Widget tree、layout、anchor、DPI scaling、widget lifecycle
- gameplay state 到 UI 的 event-driven update
- input mode、focus、mouse / keyboard / gamepad navigation
- viewport widget、world-space widget、HUD 的用途差异
- pause、settings、menu 与 gameplay input 的路由

### Audio 必须掌握

- Sound Wave、Sound Cue / MetaSound、Audio Component 的关系
- attenuation、spatialization、concurrency、loop、fade
- SoundClass、SoundMix、Submix 的职责
- one-shot、ambient、music state 和 UI sound 的生命周期

### VFX 必须掌握

- Niagara System、Emitter、module、parameter 的结构
- spawn at location、attach、pooled / persistent effect 的生命周期
- gameplay event 驱动 VFX；loop effect 必须有明确停止时机
- material、Niagara、light 和 audio 的反馈协作

### 理解并会选择

- CommonUI、MVVM
- MetaSound graph 深入设计
- Niagara simulation stage、GPU particle 和高级 data interface

### 可交给 agent

- Widget tree、常规 layout、重复 style
- SoundClass / Mix、Niagara 参数的批量接线
- 常规 hit / pickup / menu feedback 的资产装配

### 最小验收

- main menu、HUD、pause、settings 能用 keyboard / mouse 和 gamepad 完成
- attack / hit / pickup / success / failure 有一致的 animation、audio、VFX、UI 反馈
- 能排查 UI focus、widget 重复构造、声音叠加和 loop VFX 泄漏

## 10. Debugging、Profiling 与 Packaging

这是基础能力，不是项目末尾才学习的发布细节。

### 必须掌握

- PIE、Standalone、Development packaged、Shipping packaged 的环境差异
- log category、breakpoint、assert、`ensure`、Blueprint debugger
- 从 reproduction -> observation -> hypothesis -> minimal test -> fix -> regression 的诊断流程
- `stat unit`、`stat game`、`stat gpu` 和 Unreal Insights 分别回答什么问题
- game thread、render thread、GPU、GC、asset loading hitch 的基本定位
- config hierarchy、cook、stage、package 的区别
- editor-only dependency、missing cooked asset、路径 / case、save path 等常见 packaged 问题
- 性能优化必须先确定目标硬件、场景和 frame budget

### 理解并会选择

- Automation Test、Functional Test、Gauntlet 的覆盖层级
- crash log、call stack、minidump
- pak / IoStore、chunk、prerequisite、platform packaging

### 可交给 agent

- 执行 build / test / package 命令并收集 log
- 搜索 log、整理 profile capture、比较前后数据
- 创建测试样板；测试目标、fixture 和 assertion 仍需自己判断

### 最小验收

- 能从 clean build 打出 Development 和 Shipping package
- 独立诊断一次 runtime bug、asset loading bug 和性能问题
- 能判断 agent 的“已完成”是否真的经过 compile、run 和 packaged 验证

## 最小完整项目

使用一个 10–15 分钟的单机关卡贯穿学习。项目是验证知识的载体，不取代逐阶段理解。

必须包含：

- main menu、settings、pause、restart、return to menu
- level transition、respawn / checkpoint、save / load
- player、camera、input、interaction
- 一种核心玩法：战斗、解谜、收集、潜行或平台跳跃
- enemy / obstacle / puzzle object
- data-driven definition 和 soft-loaded asset
- animation、material、UI、audio、Niagara、lighting、post-process
- packaged build

验收标准：

- 能解释主要对象的 ownership、lifecycle 和通信关系
- 能画出 input -> rule -> state -> presentation -> save 的数据流
- 能说明每个核心系统为什么放在当前 class / component / subsystem
- 不依赖 agent 也能判断实现是否符合 UE 机制
- agent 操作失败时，能确定应检查哪个 subsystem、生命周期或工具
- clean build 和 packaged build 中 gameplay loop 完整
- 性能结论来自 profiling，不凭感觉优化

## AI Agent / MCP 的正确定位

### 适合交给 agent

- 查找具体 API、metadata、console command 和 Editor 菜单位置
- 创建样板 C++ / Blueprint、重复连接和常规资产配置
- 批量导入、命名、移动、设置 property、生成检查报告
- 执行 compile、Automation Test、PIE、package 并读取 log
- 根据明确 schema 创建 DataAsset / DataTable / Material Instance
- 用 MCP inspect object、asset、actor、graph 和当前 Editor state

### 不应外包理解

- class / object / asset / world 的关系
- ownership、lifecycle、reference、loading、thread boundary
- Gameplay Framework 的职责和游戏启动流程
- 状态归属、通信方式和 C++ / Blueprint 边界
- collision、animation、AI、rendering 的核心数据流
- 性能瓶颈的分类和 packaged build 的失败机制
- 游戏规则、验收条件和设计取舍

### 使用 MCP 时的检查闭环

```text
先理解本次操作涉及的 UE 概念
  -> 让 agent inspect 当前状态
  -> 明确预期变更
  -> agent 执行机械操作
  -> 回读 property / compile / run
  -> 自己解释结果为什么正确
```

不要把“tool call 成功”当作“UE 行为正确”。MCP 的价值是减少操作成本、暴露状态和加快验证，而不是替代理解。

## 学习 Prompt

```text
我正在按 ueplan.md 学习第 X 阶段的 Y 主题。
请先根据 ue.md 判断已经掌握的内容，只讲真正新增或需要纠正的部分。

要求：
1. 先讲主要结构、核心流程和心智模型
2. 解释关键对象的 owner、lifecycle、reference 和 data flow
3. 说明相邻方案的选择边界，不堆砌 API
4. 列出错误心智模型、常见坑和诊断顺序
5. 明确哪些知识必须掌握，哪些操作细节可以交给 agent / MCP
6. 给一个用于验证理解的最小实验
7. 用检查问题确认我能独立解释，而不只是让 agent 完成操作
8. 最后给适合整理进 ue.md 的速查摘要
```

每个主题学完后，向 `ue.md` 整理：

- 主要结构与核心流程
- 关键类型及职责
- ownership / lifecycle / reference
- 方案选择规则
- 易错点和诊断顺序
- 最小代码或流程示例

不要记录长篇背景、完整 API 清单和可随时由 agent 查询的 Editor 操作步骤。

## 延后内容

以下不是 UE 基础主线，出现实际需求后再学习：

- multiplayer / replication 深入机制
- GAS、Mass、复杂 PCG
- Sequencer / Control Rig 的专业制作流程
- dedicated server、platform certification、DLC / chunking
- Engine source 修改、custom rendering pass、Chaos solver 内部实现
- 大规模 World Partition production pipeline

延后不表示不重要；它们需要以前面的对象模型、Gameplay Framework、资产、线程和 profiling 知识为基础。
