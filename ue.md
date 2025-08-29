# Unreal Engine

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
      - Private
      - Public
      - MyProject.Build.cs
      - Other C++ classes in your game module
    - ModuleTest
      - Private
        - ModuleTestModule.cpp
      - Public
      - ModuleTest.Build.cs

配置顺序
https://dev.epicgames.com/documentation/en-us/unreal-engine/configuration-files-in-unreal-engine#configurationfilehierarchy

## UObjects

```cpp
AGameModeBase

ACharacter
APlayerController

UInputMappingContext
UInputAction
```
