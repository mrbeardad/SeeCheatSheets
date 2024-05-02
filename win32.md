# Win32

- [Win32](#win32)
  - [进程系统](#进程系统)
    - [进程管理](#进程管理)
      - [进程创建](#进程创建)
      - [子进程继承](#子进程继承)
      - [进程终止](#进程终止)
      - [作业](#作业)
    - [线程管理](#线程管理)
      - [线程创建](#线程创建)
      - [线程终止](#线程终止)
      - [上下文切换](#上下文切换)
      - [基本优先级](#基本优先级)
      - [动态优先级](#动态优先级)
      - [服务质量](#服务质量)
    - [动态链接](#动态链接)
    - [虚拟内存](#虚拟内存)
    - [线程管理](#线程管理-1)
    - [进程管理](#进程管理-1)
  - [资源系统](#资源系统)
  - [窗口系统](#窗口系统)
    - [渲染流程](#渲染流程)
    - [窗口结构](#窗口结构)
    - [应用窗口](#应用窗口)
      - [样式](#样式)
      - [关系](#关系)
      - [状态](#状态)
    - [消息循环](#消息循环)
      - [生命周期](#生命周期)
      - [用户输入](#用户输入)
    - [其它细节](#其它细节)
      - [DPI](#dpi)
      - [Color](#color)

## 进程系统

- 操作系统提供了“进程”与“线程”的概念，简化了应用对计算机 CPU 和内存的使用以实现“资源计算”。

- 进程是系统进行资源分配的基本单位，一个进程具有虚拟地址空间、可执行代码、系统对象的句柄、安全上下文、唯一进程标识符、环境变量、优先级类、最小和最大工作集大小，以及至少一个执行线程。

- 线程是系统进行调度执行的基本单位，同一进程中所有线程共享虚拟地址空间和系统资源，此外每个线程都维护自己的异常处理程序、调度优先级、线程本地存储、唯一的线程标识符和线程上下文。

> 详见 [MSDN](https://learn.microsoft.com/en-us/windows/win32/procthread/processes-and-threads)

### 进程管理

#### 进程创建

- `CreateProcess`

  - 对于 GUI 程序，可控制子进程第一次调用 `CreateWindow` 和 `ShowWindow` 的默认参数，如位置、大小、nCmdShow 等
  - 对于 CUI 程序，可控制子进程的控制台窗口的句柄、位置、大小等

- 用户程序入口函数

  - `main`/`wmain` for **SUBSYSTEM:CONSOLE**, 默认自动创建控制台窗口或继承父进程控制台窗口来执行程序
  - `WinMain`/`wWinMain` for **SUBSYSTEM:WINDOWS**

  ```cpp
  int main(int argc, char* argv[], char* env[]);

  int APIENTRY WinMain(HINSTANCE hInst, HINSTANCE hPrevInstance, PSTR lpCmdLine, int nShowCmd);
  ```

- exe 搜索路径（指定无路径执行文件时）

1. 进程 exe 所在目录
2. 进程当前目录
3. 32 位 Windows 系统目录（`C:\Windows\System32`）
4. 16 位 Windows 系统目录（`C:\Windows\System`）
5. Windows 系统目录（`C:\Windows`）
6. 环境变量 PATH

> - `CreateProcess`：注意若执行文件路径包含空格则需要双引号转义
> - `OpenProcess`
> - `GetCurrentProcess`：伪句柄，仅进程内有效，使用 `DuplicateHandle` 转换为真句柄
> - `GetCurrentProcessId`
> - `EnumProcesses`
> - `WTSEnumerateProcesses`
> - `GetCommandLine`
> - `CommandLineToArgv`
> - `GetEnvironmentStrings`：内容形如 `Var0=Value0\0Var1=Value1\0\0`，
> - `FreeEnvironmentStrings`
> - `ExpandEnvironmentStrings`：扩展替换变量如 `%USERPROFILE%`
> - `GetEnvironmentVariable`
> - `SetEnvironmentVariable`
> - `GetCurrentDirectory`
> - `SetCurrentDirectory`

#### 子进程继承

- 子进程**可以**继承父进程如下属性

  - 内核对象句柄
  - 环境变量
  - 当前目录
  - 控制台
  - 错误模式
  - 进程 CPU 关联性
  - 作业

- 子进程**不可**继承父进程如下属性

  - 用户对象和 GDI 对象的句柄
  - 伪句柄
  - 虚拟地址空间
  - 优先级类

- 内核对象句柄继承的必要条件
  - 创建句柄时指定 bInheritHandle 标志
  - 创建进程时指定 STARTUPINFO 中的 dwFlags 包含 STARTF_USESTDHANDLES

> - `SetHandleInformation`
> - `UpdateProcThreadAttribute`

#### 进程终止

- 进程终止原因

  - `ExitProcess`
    - C run-time library (CRT) 默认主线程退出时会调用 `ExitProcess`
    - 默认 console control handler 会在接受 CTRL+C or CTRL+BREAK 输入时调用 `ExitProcess`
  - `TerminateProcess`
  - 进程中最后一个线程终止
  - 用户关机或注销

- 进程终止结果
  - 执行 `DllMain`，除非终止原因是调用 `TerminateProcess`
  - 终止进程内所有线程
  - 释放进程资源
  - 关闭内核对象句柄
  - 设置进程退出码
  - 触发进程对象

> - `ExitProcess`
> - `TerminateProcess`
> - `GetExitCodeProcess`
> - `SetProcessShutdownParameters`：设置关机时终止优先级

#### 作业

作业用来管理一组进程，进程关联作业后不可再取消

- 控制子进程的作业继承
  - `JOB_OBJECT_LIMIT_BREAKAWAY_OK`
  - `JOB_OBJECT_LIMIT_SILENT_BREAKAWAY_OK`
- 限制资源访问
  - `JOBOBJECT_BASIC_LIMIT_INFORMATION`
  - `JOBOBJECT_BASIC_UI_RESTRICTIONS`
  - `JOBOBJECT_CPU_RATE_CONTROL_INFORMATION`
  - `JOBOBJECT_EXTENDED_LIMIT_INFORMATION`
  - `JOBOBJECT_NOTIFICATION_LIMIT_INFORMATION`
- 监视进程状态
  - `JOBOBJECT_ASSOCIATE_COMPLETION_PORT`
- 资源使用计算
  - `JOBOBJECT_BASIC_ACCOUNTING_INFORMATION`
  - `JOBOBJECT_BASIC_AND_IO_ACCOUNTING_INFORMATION`
- 作业对象销毁时终止作业内所有进程
  - `JOB_OBJECT_LIMIT_KILL_ON_JOB_CLOSE`

> - `CreateJobObject`
> - `OpenJobObject`
> - `SetInfomationJobObject`
> - `QueryInfomationJobObject`
> - `AssignProcessToJobObject`
> - `TerminateJobOnject`
> - `IsProcessInJob`

### 线程管理

#### 线程创建

- 若线程函数需要访问 C run-time library (CRT)，则应该使用 `_beginthreadex` 而非 `CreateThread` 来保证线程安全
- 线程栈最大默认 1M，可通过控制编译时链接器参数或运行时 `CreateThread` 参数来改变，大小向上取整 1M
- 在新线程中执行 `DllMain`

> - `CreateThread`：安全属性、栈大小、暂停状态
> - `CreateRemoteThread`
> - `OpenThread`

#### 线程终止

- 线程终止原因

  - 线程函数返回
  - `ExitThread`
  - `ExitProcess`
  - `TerminateThread`
  - `TerminateProcess`

- 线程终止结果
  - 执行 `DllMain`，除非终止原因是 `TerminateThread` 或 `TerminateProcess`
  - 释放线程拥有的资源，如 windows 和 hooks
  - 设置线程退出码
  - 触发线程对象
  - 如果线程是进程里唯一的线程，则终止进程

> - `ExitThread`
> - `TerminateThread`
> - `GetExitCodeThread`

#### 上下文切换

- 上下文切换原因
  - 时间片到期，大概 20 ms
  - 主动放弃剩余时间片
  - 被高优先级线程抢占
  - 进入同步等待状态

> - `SuspendThread`
> - `ResumeThread`
> - `Sleep`
> - `SwitchToThread`

#### 基本优先级

|                               | IDLE_PRIORITY_CLASS | BELOW_NORMAL_PRIORITY_CLASS | NORMAL_PRIORITY_CLASS | ABOVE_NORMAL_PRIORITY_CLASS | HIGH_PRIORITY_CLASS | REALTIME_PRIORITY_CLASS |
| ----------------------------- | ------------------- | --------------------------- | --------------------- | --------------------------- | ------------------- | ----------------------- |
| THREAD_PRIORITY_IDLE          | 1                   | 1                           | 1                     | 1                           | 1                   | 16                      |
| THREAD_PRIORITY_LOWEST        | 2                   | 4                           | 6                     | 8                           | 11                  | 22                      |
| THREAD_PRIORITY_BELOW_NORMAL  | 3                   | 5                           | 7                     | 9                           | 12                  | 23                      |
| THREAD_PRIORITY_NORMAL        | 4                   | 6                           | 8                     | 10                          | 13                  | 24                      |
| THREAD_PRIORITY_ABOVE_NORMAL  | 5                   | 7                           | 9                     | 11                          | 14                  | 25                      |
| THREAD_PRIORITY_HIGHEST       | 6                   | 8                           | 10                    | 12                          | 15                  | 26                      |
| THREAD_PRIORITY_TIME_CRITICAL | 15                  | 15                          | 15                    | 15                          | 15                  | 31                      |

- 通过设置进程优先级和线程优先级，系统确认线程使用的基本优先级
- 基本优先级保留给 zero-page thread，它负责将 free page 置零
- 注意 `REALTIME_PRIORITY_CLASS` 优先级会中断系统线程，比如键鼠输入、磁盘冲刷等

> - `GetPriorityClass`
> - `SetPriorityClass`
> - `GetThreadPriority`
> - `SetThreadPriority`

#### 动态优先级

- 动态优先级用来最终确认线程的执行优先级
- 系统在以下情况会动态提升基本优先级 0 - 15 的线程
  - `NORMAL_PRIORITY_CLASS` 优先级的进程成为前台进程时，提升其优先级大于或等于任何后台进程
  - 当窗口接受到用户输入时，提升窗口所属的线程的优先级
  - 当线程同步等待的条件被满足时，提升该线程的优先级
  - 优先级反转：高优先级线程等待低优先级线程的资源，而中优先级线程一直抢占低优先级线程，导致高优先级线程被中优先级线程阻塞。系统会自动检测这种情况，并动态提升低优先级线程至所有等待它的线程中的最大优先级
- 动态提升之后，动态优先级每个时间片降低 1 级，且绝不低于基本优先级

> - `GetProcessPriorityBoost`
> - `SetProcessPriorityBoost`
> - `GetThreadPriorityBoost`
> - `SetThreadPriorityBoost`

#### 服务质量

服务质量 (Quality of Service) 会影响线程运行的处理器核心和功率，具体见 [MSDN](https://learn.microsoft.com/en-us/windows/win32/procthread/quality-of-service)

### 动态链接

```cpp
// mydll.h
#pragma once

#ifdef MYDLL_EXPORT
#define MYDLL_API __declspec(dllexport)
#else
#define MYDLL_API __declspec(dllimport)
#endif

#ifdef __cplusplus
extern "C" {
#endif

MYDLL_API int __stdcall my_func(LPCWSTR lpszMsg);


#ifdef __cplusplus
} // extern "C"
#endif
```

**dll 的生命周期**：

1. 动态链接：加载时（加载进程 exe）或运行时（`LoadLibrary`）
2. 内存映射
3. 符号解析
4. 调用 c/c++ runtime library 动态库入口，负责初始化全局变量等
5. 调用 DllMain

   ```cpp
   // 进程内所有 DllMain 的执行都需要获取同一个互斥锁（进程唯一）
   BOOL WINAPI DllMain(HINSTANCE hinstDLL, DWORD fdwReason, LPVOID lpvReserved) {
     switch (fdwReason) {
       case DLL_PROCESS_ATTACH:
         // 首次加载时，根据加载方式的不同：
         // 加载时动态链接：在主线程执行，lpvReserved 为 non-NULL，返回 FALSE 表示发生错误而终止进程
         // 运行时动态链接：在调用 LoadLibrary 的线程执行，lpvReserved 为 NULL，返回 FALSE 表示 LoadLibrary 返回 FALSE
         break;
       case DLL_THREAD_ATTACH:
         // 创建新线程时，在运行其线程函数前执行
         // 不会在 DLL_PROCESS_ATTACH 的线程执行
         // 不会在已创建的线程中执行；
         // 不会用该参数执行调用了 DisableThreadLibraryCalls 的模块的 DllMain
         break;
       case DLL_THREAD_DETACH:
         // 线程终止时，在运行其线程函数返回后执行
         // 可能没有对应的 DLL_THREAD_ATTACH，可能因为加载时线程已创建或直接调用了 ExitThread；
         // 不会用该参数执行调用了 DisableThreadLibraryCalls 的模块的 DllMain
         break;
       case DLL_PROCESS_DETACH:
         // 模块卸载时，根据卸载方式的不同：
         // 进程退出：在主线程或调用了 ExitProcess 的线程中执行，lpvReserved 为 non-NULL
         // 手动卸载：在调用了 FreeLibrary 的线程中执行，lpvReserved 为 NULL
         break;
     }
     return TRUE;
   }
   ```

**dll 标准搜索路径**：（适用于相对路径和无路径文件名）

> 更详细 dll 搜索路径见 [MSDN](https://learn.microsoft.com/en-us/windows/win32/dlls/dynamic-link-library-search-order)

1. DLL Redirection.
2. API sets.
3. SxS manifest redirection.
4. Loaded-module list.
5. Known DLLs.
6. The package dependency graph of the process. Windows 11, version 21H2 (10.0; Build 22000), and later.
7. 进程 exe 所在目录
8. 32 位 Windows 系统目录（`C:\Windows\System32`）
9. 16 位 Windows 系统目录（`C:\Windows\System`）
10. Windows 系统目录（`C:\Windows`）
11. 进程当前目录
12. 环境变量 PATH

**dll 注入**：

- 注册表`Computer\HKEY_LOCAL_MACHINE\SOFTWARE\Microsoft\Windows NT\CurrentVersion\Windows\AppInit_DLLs`
  - 设置空格分隔的 dll 列表，它们会被 User32.dll 加载，注意此时许多 dll 还未被加载
  - 所有 GUI 程序都会加载 User32.dll
- `SetWindowsHookEx`:
  - 给指定线程或同 Desktop 下所有线程添加 Hook，Hook 会在下次获取窗口消息时安装，会先加载 dll 然后注册 Hook
  - 添加全局 Hook 时，异构线程的消息会送给调用安装 Hook 的线程处理，所以需要保证安装 Hook 的线程能正常处理消息
  - 安装 Hook 的进程退出时会卸载 Hook，卸载 Hook 同时会递减 dll 的引用计数
- 远程线程
- 木马 dll

> 相关接口
>
> - `LoadLibrary`
> - `LoadLibraryEx`: 可设置修改标准搜索路径、仅加载资源数据，通常不能与`LoadLibrary`混用
> - `GetProcAddress`
> - `FreeLibrary`
> - `FreeLibraryAndExitThread`: FreeLibrary 和 ExitThread 可能都会调用 DllMain，虽然 DllMain 加了锁，但是并未验证 DllMain 的有效性（dll 被卸载）
> - `__ImageBase`: 由链接器创建的变量，位于该模块的基地址
> - `GetModuleHandle`: 不递增引用计数
> - `GetModuleHandleEx`: 默认递增引用计数，可设置 dll 直到进程终止前绝不卸载
> - `GetModuleFileName`
>
> 关于 Windows 运行时环境和打包部署建议见 [MSDN](https://learn.microsoft.com/en-us/cpp/windows/deployment-concepts?view=msvc-170)

### 虚拟内存

- 虚拟地址空间

| 分区          | x86                     | x64                                       |
| ------------- | ----------------------- | ----------------------------------------- |
| 空指针赋值区  | 0x00000000 - 0x0000FFFF | 0x00000000'00000000 - 0x00000000'0000FFFF |
| 用户模式分区  | 0x00010000-0x7FFEFFFF   | 0x00000000'00010000 - 0x000007FF'FFFEFFFF |
| 64KB 禁入分区 | 0x7FFF0000 - 0x7FFFFFFF | 0x000007FF'FFFF0000 - 0x000007FF'FFFFFFFF |
| 内核模式分区  | 0x80000000 - 0xFFFFFFFF | 0x00000800'00000000 - 0xFFFFFFFF'FFFFFFFF |

- 页面：CPU 进行虚拟地址翻译的粒度，故系统进行内存管理和分配的粒度必须是其整数倍，x86/x64 页面大小为 4K，Windows 用户内存分配粒度为 64K

- 页面状态

  - `MEM_FREE`: 该地址尚未被分配，无法访问
  - `MEM_RESERVE`: 该地址已被分配，但还未调拨存储器，无法访问
  - `MEM_COMMIT`: 该地址已被分配，且已调拨存储器 (storage)，第一次访问时系统会为其准备物理内存页面和对应数据

- 页面类型

  - `MEM_PRIVATE`:
    - 后备存储器为页交换文件 (paging file)
    - 第一次访问时无需从页交换文件中加载页面，直接申请物理内存页面并置零
  - `MEM_IMAGE`:
    - 后备存储器为映像文件，如 exe 或 dll，
    - 映像文件为 PE 格式，内部有多个不同的段，加载不同的段时会使用不同的页面保护属性
  - `MEM_MAPPED`:
    - 后备存储器可以为页交换文件（如共享内存）或其它磁盘文件（如文件映射）

- 页面保护

  - `PAGE_NOACCESS`
  - `PAGE_READONLY`
  - `PAGE_READWRITE`
  - `PAGE_WRITECOPY`
  - `PAGE_EXECUTE`
  - `PAGE_EXECUTE_READ`
  - `PAGE_EXECUTE_READWRITE`
  - `PAGE_EXECUTE_WRITECOPY`
  - `PAGE_GUARD`: 设置一次性内存访问异常处理

- 地址访问

  - CPU 中的内存管理单元（MMU）负责根据页表基址寄存器（PTBR）存储的页表基址从翻译后备缓冲器（TLB）获取虚拟地址对应的页表表项（PTE）从而翻译为物理地址
  - 操作系统为每个进程维护一个多级页表，最后一级页表表项记录虚拟地址对应的物理地址，其它级页表表项记录下一级页表基地址，通常只有一级页表常驻内存
  - 当访问的地址没有在内存中时，触发缺页异常，控制流交给操作系统处理异常：
    - 将对应的页面从其后备存储器中加载到内存
    - 当内存中无空闲页面时，根据某种缓存驱逐策略来选择使用页面，若为脏页则先将其冲刷到其后备存储器再使用

> 相关接口
>
> - `GetSystemInfo`: CPU 硬件信息
> - `GlobalMemoryStatusEx`: 系统内存情况
> - `VirtualQuery`: 该进程虚拟内存信息
> - `VirtualAlloc`: 申请内存，设置页面状态
> - `VirtualFree`: 释放内存
> - `VirtualProtect`: 设置页面保护
> - `CreateFile`
> - `CreateFileMapping`
> - `MapViewOfFile`
> - `HeapCreate`
> - `HeapDestroy`
> - `HeapAlloc`
> - `HeapFree`
> - `HeapReAlloc`
> - `GetProcessHeap`: 每个进程有一个默认的线程安全的堆
> - `GetProcessHeaps`
> - `GetProcessWorkingSetSize `

### 线程管理

- 线程是调度 CPU 控制流的基本单位

- 调度优先级：只要存在高优先级的线程处于可调度状态，就会先运行高优先级线程，以下情况可动态提升线程优先级

  - 接受新消息
  - 前台进程中的线程
  - 长时间处于饥饿状态的低优先级线程

- CPU 关联性

- 线程同步
  - 为什么需要同步？
    - 防止读取并操作数据时数据被修改（加锁）
    - 等待状态被正确初始化（等待事件）
  - 用户模式
    - [联锁变量](https://learn.microsoft.com/en-us/windows/win32/sync/interlocked-variable-access)
    - [关键段](https://learn.microsoft.com/en-us/windows/win32/sync/critical-section-objects)
    - [读写锁](https://learn.microsoft.com/en-us/windows/win32/sync/slim-reader-writer--srw--locks)
    - [条件变量](https://learn.microsoft.com/en-us/windows/win32/sync/condition-variables)
    - [屏障](https://learn.microsoft.com/en-us/windows/win32/sync/synchronization-barriers)
  - 内核模式
    - [互斥量](https://learn.microsoft.com/en-us/windows/win32/sync/mutex-objects)：等于 0 时为触发状态，等于 TID 时为非触发状态
    - [信号量](https://learn.microsoft.com/en-us/windows/win32/sync/semaphore-objects)：大于 0 时为触发状态，等于 0 时为非触发状态
    - [事件](https://learn.microsoft.com/en-us/windows/win32/sync/event-objects)
    - [计时器](https://learn.microsoft.com/en-us/windows/win32/sync/waitable-timer-objects)

> 相关接口：
>
> - `SuspendThread`
> - `ResumeThread`
> - `Sleep`
> - `SwitchToThread`: 相对`Sleep`，允许切换低优先级线程
> - `GetThreadTimes`
> - `GetProcessTimes`
> - `GetTickCount64`
> - `SetPriorityClass`
> - `SetThreadPriority`
> - `SetProcessAffinityMask`
> - `SetThreadAffinityMaxk`
> - `WaitForSingleObjectEx`
> - `WaitForMultipleObjectsEx`
> - `MsgWaitForMultipleObjectsEx`
> - `SignalObjectAndWait`

### 进程管理

- 作业（内核对象）：

  - 限制一组进程对一些资源的访问，比如 CPU 的使用、UI 限制、安全限制等
  - 将进程移入作业后不可移出
  - 子进程默认也在作业中
  - 可以终止作业中所有进程
  - 可以监听作业中进程的状态

- Session
  - Window Station: contains a clipboard, an atom table, and one or more desktop objects
    - Desktop: A desktop has a logical display surface and contains user interface objects such as windows, menus, and hooks; it can be used to create and manage windows.

> 相关接口：
>
> - `OpenJobObject`
> - `SetInfomationJobObject`
> - `QueryInfomationJobObject`
> - `IsProcessInJob`
> - `AssignProcessToJobObject`
> - `TerminateJobOnject`
> - `GetQueuedCompletionStatus`

## 资源系统

- 系统启动时默认创建 Session 0 专门用于运行 Services，其中的进程无法访问终端交互设备
- 第一个登录的用户通常在 Session 1，其中的进程可以访问终端交互设备
- 内核对象命名空间分为 Global 和 Local，前者跨多个会话，后者仅用于单个会话

- 用户对象
- GDI 对象
- 内核对象

进程句柄表：

- 索引：`HANDLE`
- 内核对象指针
  - 引用计数
  - 安全描述符
  - 名字
    - 全局命名空间：`Global\`前缀，服务会话（sesson 0）默认
    - 会话命名空间：`Local\`前缀，用户会话默认
- 访问掩码：这个句柄的访问权限
- 属性标志：如 Protect, Inherit 等

> 相关接口：
>
> - `GetHandleInformation`
> - `SetHandleInformation`
> - `DuplicateHandle`

## 窗口系统

### 渲染流程

[Windows Graphics Overview](https://learn.microsoft.com/en-us/windows/win32/learnwin32/overview-of-the-windows-graphics-architecture)

1. Window
2. Graphics API: GDI, Direct2D, DirectWrite, OpenGL, Vulkan ...
3. Paint
4. Composite
5. Rasterize
6. Display

### 窗口结构

- Window Station: Winsta0

  - Desktop: Default, ScreenSaver, and Winlogon

    - Monitor

      - Desktop Window
      - Taskbar

        - Taskbar Button
          - Icon and Label
          - Overlay Icon
          - Progress Bar
          - Jump List
          - Thumbnail Toolbar
        - Notification Area
          - Icon
          - Hover -> Tooltip
          - Left click -> Popup Window
          - Left double-click -> Primary UI
          - Right-click -> Context Menu

      - Application Window
        - Non-client Area
        - Client Area

### 应用窗口

#### 样式

- `WS_CAPTION`: title bar and unsizing border
- `WS_SYSMENU`: application icon, window menu and close button (requrie `WS_CAPTION`)
- `WS_MINIMIZEBOX`: minimize button
- `WS_MAXIMIZEBOX`: maximize and restore button
- `WS_BORDER`: unsizing border
- `WS_THICKFRAME`: sizing border
- `WS_VSCROLL`: vertical scroll bar
- `WS_HSCROLL`: horizontal scroll bar
- Overlapped
  - `WS_OVERLAPPED`: title bar and unsizing border
  - `WS_OVERLAPPEDWINDOW`: `WS_OVERLAPPED | WS_CAPTION | WS_SYSMENU | WS_MINIMIZEBOX | WS_MAXIMIZEBOX | WS_THICKFRAME`
- Popup
  - `WS_POPUP`: only client area
  - `WS_POPUPWINDOW`: `WS_POPUP | WS_BORDER | WS_SYSMENU`
  - `WS_POPUPWINDOW | WS_CAPTION`
- Child
  - `WS_CHILD`: only client area (cannot have a menu bar, cannot be used with the `WS_POPUP`)

#### 关系

- Child Window

  - 一般用于控件窗口来接收用户输入以及布局
  - 使用基于父窗口客户区的坐标系
  - 跟随父窗口一同显示、移动、销毁
  - 超出父窗口客户端部分将被裁剪
  - `WS_CLIPCHILDREN`: 父窗口的绘制不再覆盖该子窗口
  - `WS_CLIPSIBLINGS`: 同级窗口的绘制不再覆盖该子窗口
  - 禁用状态的子窗口的消息直接发送给父窗口
  - 父窗口将`hMenu`置为子窗口标识符来识别子窗口消息
  - `GetParent()`
  - `SetParent()`: 移动到新父窗口中，父窗口置空则成为 Top-Level Window
  - `IsChild()`: 判断是否为子窗口或后代窗口
  - `EnumChildWindows()`: 遍历子窗口与后代窗口

- Owned Window

  - `pWndParent` set to a Overlapped/Pop-up Window
  - 总是在拥有者窗口上方
  - 当拥有者窗口最小化时隐藏，当拥有者窗口恢复时显示
  - 跟随拥有者窗口一起销毁

#### 状态

![coordinates](./images/coordinates.png)

- `GetWindowLongPtr()`/`SetWindowLongPtr()`

- 坐标系中的位置与大小：
  - `CW_USEDEFAULT`：默认位置与大小
  - `SetWindowPlacement()`：设置最小化位置、最大化位置与还原位置与大小，还有显示状态
  - `SetWindowPos()`：设置位置与大小，还有显示状态
  - `MoveWindow()`：仅设置位置与大小
  - `GetWindowRect()`：获取基于屏幕坐标系的位置与大小
  - `ScreenToClient()`/`MapWindowPoints()`：将子窗口的屏幕坐标系 RECT 转换为父窗口客户区坐标系 RECT
  - `WM_GETMINMAXINFO`：当位置与大小改变时发送该消息，`MINMAXINFO`包含最大化位置与尺寸、最大最小可变尺寸
  - `WM_WINDOWPOSCHANGING`：当位置、大小、Z-Order、显示状态改变时发送该消息，`WINDOWPOS`包含新的位置、大小、Z-Order 和显示状态
- Z-Order：
  - `WS_EX_TOPMOST`：每级 Z 轴分两段 —— 普通与置顶
  - `GetNextWindow()`
  - `SetWindowPos()`
  - `BringWindowToTop()`：将窗口置于同段 Z 轴顶部
  - `GetTopWindow()`：获取指定父窗口的 Z 轴最高的子窗口
- 显示状态：
  - `WS_VISIBLE`
  - `IsWindowVisible()`
  - `ShowWindow()`
  - `SetWindowLong()`
  - `ShowOwnedPopups()`
  - `WM_SHOWWINDOW`
- 最大化最小化状态：
  - `WS_MAXIMIZE`/`WS_MINIMIZE`
  - `IsZoomed()`/`IsIconic()`
  - `OpenIcon()`
  - `CloseWindow()`
  - `ShowWindow()`
  - `SetWindowPlacement()`
  - `GetWindowPlacement()`
- 禁用状态：
  - `WS_DISABLED`
  - `EnableWindow()`
- 前台状态与激活状态：
  - `GetActiveWindow()`/`SetActiveWindow()`(thread)
  - `GetForegroundWindow()`/`SetForegroundWindow()`(global)

### 消息循环

[Your First Windows Program](https://learn.microsoft.com/en-us/windows/win32/learnwin32/your-first-windows-program)

1. RegisterClassEx：exe 注册的 Class 在退出后销毁，dll 注册的样式需要手动销毁，class 由 classname 和 hinstance 唯一确定
2. CreateWindowEx
3. ShowWindow
4. GetMessage
5. DispatchMessage
6. WindowProc

#### 生命周期

1. WM_NCCREATE
2. WM_NCCALCSIZE
3. WM_CREATE
4. WM_PAINT
5. WM_CLOSE
6. WM_DESTROY
7. WM_NCDESTROY
8. WM_QUIT

#### 用户输入

[User Input](https://learn.microsoft.com/en-us/windows/win32/learnwin32/module-4--user-input)

### 其它细节

#### DPI

[DPI and device-independent pixels](https://learn.microsoft.com/en-us/windows/win32/learnwin32/dpi-and-device-independent-pixels)

DPI Awareness:

- Unaware: 按照系统 DPI 缩放比例，自动将绘制后的窗口缩放，可能会导致模糊
- System Aware
- Per-Monitor Aware

DIP:

- GDI 绘制不支持缩放，但是字体渲染支持缩放
- Direct2D 绘制和 DirectWrite 字体渲染均支持缩放
- 注意：窗口和鼠标信息通常为物理像素，而非自动缩放的逻辑像素

#### Color

D2D Color 用浮点数表示 RGBA，但实现上有三种表示方式：

- `DXGI_FORMAT_B8G8R8A8_UNORM`
- `DXGI_FORMAT_R8G8B8A8_UNORM`
- `DXGI_FORMAT_A8_UNORM`

Alpha mode:

- `D2D1_ALPHA_MODE_IGNORE`: 忽略 alpha 可提高性能
- `D2D1_ALPHA_MODE_STRAIGHT`: RGB 通道值乘以 A
- `D2D1_ALPHA_MODE_PREMULTIPLIED`" RGB 通道值已经是乘以 A 后的值了
