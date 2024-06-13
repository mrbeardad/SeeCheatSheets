# Win32

- [Win32](#win32)
  - [进程系统](#进程系统)
    - [进程管理](#进程管理)
      - [进程创建](#进程创建)
      - [继承](#继承)
      - [进程终止](#进程终止)
      - [作业](#作业)
    - [线程管理](#线程管理)
      - [线程创建](#线程创建)
      - [线程终止](#线程终止)
      - [线程调度](#线程调度)
        - [上下文切换](#上下文切换)
        - [基本优先级](#基本优先级)
        - [动态优先级](#动态优先级)
        - [服务质量](#服务质量)
        - [CPU 亲和性](#cpu-亲和性)
      - [线程同步](#线程同步)
    - [动态链接](#动态链接)
    - [虚拟内存](#虚拟内存)
    - [异常处理](#异常处理)
  - [IO 系统](#io-系统)
    - [句柄表](#句柄表)
    - [命名空间](#命名空间)
    - [访问控制](#访问控制)
      - [隔离性](#隔离性)
      - [访问控制模型](#访问控制模型)
      - [强制可信控制](#强制可信控制)
      - [用户访问控制](#用户访问控制)
    - [注册表](#注册表)
    - [文件系统](#文件系统)
      - [磁盘](#磁盘)
      - [文件](#文件)
      - [路径](#路径)
      - [读写](#读写)
    - [IPC 机制](#ipc-机制)
      - [套接字](#套接字)
      - [管道](#管道)
      - [数据拷贝](#数据拷贝)
      - [共享内存](#共享内存)
      - [总结](#总结)
    - [异步 IO](#异步-io)
  - [窗口系统](#窗口系统)
    - [桌面环境](#桌面环境)
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
  - [其他](#其他)
    - [头文件宏](#头文件宏)
    - [字符集](#字符集)

## 进程系统

- 操作系统提供了“进程”与“线程”的概念，简化了程序对系统中“计算资源”的使用，如 CPU 和内存等。

- 进程是系统进行内存资源分配的基本单位，一个进程具有唯一的进程标识符、优先级类、虚拟地址空间、异常处理程序、内核对象句柄、安全上下文、环境变量、命令行参数等，并且至少有一个执行线程。

- 线程是系统进行 CPU 资源分配的基本单位，同一进程中所有线程共享虚拟地址空间等进程资源，此外每个线程负责维护自己的唯一的线程标识符、线程优先级、线程本地存储、线程上下文等。

### 进程管理

> 参考 [Processes and Threads](https://learn.microsoft.com/en-us/windows/win32/procthread/processes-and-threads)

#### 进程创建

1. `CreateProcess`

   - 对于 GUI 程序，可控制子进程第一次调用 `CreateWindow` 和 `ShowWindow` 的默认参数，如位置、大小、nCmdShow 等
   - 对于 CUI 程序，可控制子进程的控制台窗口的句柄、位置、大小等

2. C Run-Time library (CRT) 入口函数，负责初始化全局变量等，然后调用用户程序入口函数

3. 用户程序入口函数

   - `main`/`wmain` for **SUBSYSTEM:CONSOLE**, 默认自动创建控制台窗口或继承父进程控制台窗口来执行程序
   - `WinMain`/`wWinMain` for **SUBSYSTEM:WINDOWS**

   ```cpp
   int main(int argc, char* argv[], char* env[]);

   int APIENTRY WinMain(HINSTANCE hInst, HINSTANCE hPrevInstance, PSTR lpCmdLine, int nShowCmd);
   ```

- exe 搜索路径（指定无路径的文件名时）

1. 进程 exe 文件所在目录
2. 进程当前目录
3. 32 位 Windows 系统目录（`C:\Windows\System32`）
4. 16 位 Windows 系统目录（`C:\Windows\System`）
5. Windows 系统目录（`C:\Windows`）
6. 环境变量 PATH

> - `CreateProcess`
> - `OpenProcess`
> - `GetProcessId`
> - `GetCurrentProcess`：伪句柄，仅进程内有效
> - `GetCurrentProcessId`
> - `EnumProcesses`
> - `GetCommandLine`
> - `CommandLineToArgv`
> - `GetEnvironmentStrings`：格式为 `Var0=Value0\0Var1=Value1\0\0`
> - `FreeEnvironmentStrings`
> - `ExpandEnvironmentStrings`：扩展替换变量如 `%USERPROFILE%`
> - `GetEnvironmentVariable`
> - `SetEnvironmentVariable`
> - `GetCurrentDirectory`
> - `SetCurrentDirectory`

#### 继承

- 子进程**可以**继承父进程如下属性

  - 环境变量
  - 当前目录
  - 控制台
  - 内核对象句柄（句柄具有 `bInheritHandle` 且 `bInheritHandles` 设置 `true`）
  - 错误模式
  - 进程 CPU 亲和性（`dwCreationFlags` 设置 `INHERIT_PARENT_AFFINITY`）
  - 作业

- 子进程**不可**继承父进程如下属性

  - 用户对象和 GDI 对象的句柄
  - 伪句柄
  - 虚拟地址空间
  - 优先级类

> - `SetHandleInformation`
> - `UpdateProcThreadAttribute`

#### 进程终止

- 进程终止原因

  - `ExitProcess`

    - 进程中最后一个线程终止（默认 CRT 实现主线程退出时终止进程）
    - 用户模式中发生的硬件异常或软件异常未被捕获处理时
    - 调用 `ExitWindowsEx` 时
      - [用户注销或关机时](https://learn.microsoft.com/en-us/windows/win32/shutdown/shutting-down)
      - 内核模式中发生的硬件异常或软件异常未被捕获时（蓝屏）

  - `TerminateProcess`

- 进程终止结果

  1. 终止进程内所有线程，并释放线程资源
  2. 主线程退出前，卸载所有动态模块
  3. 释放其他进程资源，如关闭内核对象句柄
  4. 设置进程退出码
  5. 触发进程对象

- C++ 静态生命周期的变量
  - 仅能保证同一编译单元中的构造和析构顺序，且析构顺序与构造顺序相反
  - 构造
    1. `static`（模块加载）
    2. `thread_local`（线程加载）
    3. in-block `static`/`thread_local`（首次调用函数）
  - 析构
    1. `thread_load`（线程退出）
    2. in-block `thread_local`（线程退出）
    3. `static`（模块卸载）
    4. in-block `static`（模块卸载）
    5. `std::atexit` 保证在注册时就已初始化的任何 `static` 销毁之前调用
  - **注意：无法保证变量的析构函数一定会被调用**

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
- 资源使用情况
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

> 参考 [Processes and Threads](https://learn.microsoft.com/en-us/windows/win32/procthread/processes-and-threads)

#### 线程创建

- **若线程函数需要访问 CRT，则应该使用 `_beginthreadex` 而非 `CreateThread` 来保证线程安全**
- 线程栈最大默认 1M，可通过控制编译时链接器参数或运行时 `CreateThread` 参数来改变，大小向上取整 1M

> - `CreateThread`：安全属性、栈大小、暂停状态
> - `CreateRemoteThread`: 在其他进程中创建线程
> - `OpenThread`
> - `GetThreadId`
> - `GetCurrentThread`：伪句柄，仅进程内有效，使用 `DuplicateHandle` 转换为真句柄
> - `GetCurrentThreadId`

#### 线程终止

- 线程终止原因

  - `ExitThread`
    - 线程函数返回
  - `ExitProcess`
  - `TerminateThread`
  - `TerminateProcess`

- 线程终止结果
  - 释放线程拥有的资源，如 TLS (Thread Local Storage), windows, menus, hooks
  - 设置线程退出码
  - 触发线程对象
  - 如果线程是进程里唯一的线程，则终止进程

> - `ExitThread`
> - `TerminateThread`
> - `GetExitCodeThread`

#### 线程调度

##### 上下文切换

- 上下文切换原因
  - 时间片到期，大概 20 ms
  - 主动放弃剩余时间片
  - 被高优先级线程抢占
  - 进入同步等待状态或暂停状态

> - `SuspendThread`
> - `ResumeThread`
> - `Sleep`
> - `SwitchToThread`: 相对 `Sleep(0)`，允许切换低优先级线程
> - `GetThreadTimes`
> - `GetProcessTimes`

##### 基本优先级

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
- 基本优先级 0 保留给 zero-page thread，它负责将 free page 置零
- 注意 `REALTIME_PRIORITY_CLASS` 优先级会中断系统线程，比如键鼠输入、磁盘冲刷等

> - `GetPriorityClass`
> - `SetPriorityClass`
> - `GetThreadPriority`
> - `SetThreadPriority`

##### 动态优先级

- 动态优先级用来最终确认线程的执行优先级
- 系统在以下情况会动态提升基本优先级 0 - 15 的线程
  - `NORMAL_PRIORITY_CLASS` 优先级的进程成为前台进程时，提升其优先级大于或等于任何后台进程
  - 当窗口接受到用户输入时，提升窗口所属的线程的优先级
  - 当线程同步等待的条件被满足时，提升该线程的优先级
  - 优先级反转：高优先级线程等待低优先级线程的资源，而中优先级线程一直抢占低优先级线程，导致高优先级线程被中优先级线程阻塞。系统会自动检测这种情况，并动态提升低优先级线程至所有等待它的线程中的最大优先级
  - 当低优先级线程长时间处于饥饿状态，提升该线程的优先级
- 动态提升之后，动态优先级每个时间片降低 1 级，且绝不低于基本优先级

> - `GetProcessPriorityBoost`
> - `SetProcessPriorityBoost`
> - `GetThreadPriorityBoost`
> - `SetThreadPriorityBoost`

##### 服务质量

服务质量 (Quality of Service) 会影响线程运行的处理器核心和功率，具体见 [QoS](https://learn.microsoft.com/en-us/windows/win32/procthread/quality-of-service)

##### CPU 亲和性

> 参考
>
> - [Processor Groups](https://learn.microsoft.com/en-us/windows/win32/procthread/processor-groups)
> - [CPU Sets](https://learn.microsoft.com/en-us/windows/win32/procthread/cpu-sets)

利用 CPU 亲和性可以限制进程或线程运行在制定的 CPU 上

- 操作系统可包含多个物理处理器 (physical processor)
- 物理处理器可包含多个核心 (core)
- 核心可包含多个逻辑处理器 (logical processor)
- 每 64 个逻辑处理器为一个处理器组 (processor group)
- Windows 11 之后线程可以跨多个处理器组，默认优先在主组中运行

#### 线程同步

- 为什么需要同步？
  - 保护共享数据被同时读写
  - 等待某个事件发生
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
  - 进程
  - 线程

> - `WaitForSingleObjectEx`
> - `WaitForMultipleObjectsEx`
> - `MsgWaitForMultipleObjectsEx`
> - `SignalObjectAndWait`

### 动态链接

> 参考 [Dynamic-Link Libraries](https://learn.microsoft.com/en-us/windows/win32/dlls/dynamic-link-libraries)

```cpp
// mydll.h
#pragma once

#ifdef MYDLL_EXPORT
#define MYDLL_API __declspec(dllexport)
#else
#define MYDLL_API __declspec(dllimport)
#endif

#ifdef __cplusplus
extern "C" { // 使用默认调用约定，且禁用名字篡改
#endif

MYDLL_API int my_func1(LPCWSTR lpszMsg);

MYDLL_API int __stdcall my_func2(LPCWSTR lpszMsg); // 使用 __stdcall 调用约定和名字篡改

#ifdef __cplusplus
} // extern "C"
#endif
```

dll 的生命周期：

1. 动态链接：
   - 加载时，即创建进程时，此时仅保证 Kernel32.dll 已被加载
   - 运行时，即调用 `LoadLibrary` 时，注意提前卸载模块可能导致某些地方仍在使用模块内的函数
2. 内存映射
3. 符号解析
4. 调用 CRT 动态库入口，负责构造和析构全局变量等
5. 调用 DllMain

   ```cpp
   // 进程内所有 DllMain 的执行都需要获取同一个互斥锁（进程唯一）
   // 所以谨慎在 DllMain 中调用 LoadLibrary, FreeLibrary, CreateThread, ExitThread 等函数
   BOOL WINAPI DllMain(HINSTANCE hinstDLL, DWORD fdwReason, LPVOID lpvReserved) {
     switch (fdwReason) {
       case DLL_PROCESS_ATTACH:
         // 首次加载时，根据加载方式的不同：
         // - 加载时动态链接：在主线程执行，lpvReserved 为 non-NULL，返回 FALSE 表示发生错误而终止进程
         // - 运行时动态链接：在调用 LoadLibrary 的线程执行，lpvReserved 为 NULL，返回 FALSE 表示 LoadLibrary 返回 FALSE
         break;
       case DLL_PROCESS_DETACH:
         // 模块卸载时，根据卸载方式的不同：
         // - 进程退出：在调用了 ExitProcess 的线程中执行，lpvReserved 为 non-NULL
         // - 手动卸载：在调用了 FreeLibrary 的线程中执行，lpvReserved 为 NULL
         // - 注意，调用了 TerminateThread 或 TerminateProcess 不会调用 DllMain
         break;
       case DLL_THREAD_ATTACH:
         // - 创建新线程时，在运行其线程函数前执行
         // - 不会在 DLL_PROCESS_ATTACH 的线程执行
         // - 不会在加载时已经存在的线程中执行
         // - 若线程调用了 DisableThreadLibraryCalls 则不会执行
         break;
       case DLL_THREAD_DETACH:
         // - 线程终止时，在运行其线程函数返回后执行
         // - 可能没有对应的 DLL_THREAD_ATTACH，因为加载时线程已存在
         // - 若线程调用了 DisableThreadLibraryCalls 则不会执行
         // - 注意，调用了 TerminateThread 或 TerminateProcess 不会调用 DllMain
         break;
     }
     return TRUE;
   }
   ```

dll 标准搜索路径：（适用于相对路径和无路径文件名）

> 更详细 dll 搜索路径见 [Dll search order](https://learn.microsoft.com/en-us/windows/win32/dlls/dynamic-link-library-search-order)；  
> 关于 Windows 运行时环境和打包部署建议见 [Deployment](https://learn.microsoft.com/en-us/cpp/windows/deployment-in-visual-cpp?view=msvc-170)

1. DLL Redirection.
2. API sets.
3. SxS manifest redirection.
4. Loaded-module list.
5. Known DLLs.
6. The package dependency graph of the process.
7. 进程 exe 所在目录
8. 32 位 Windows 系统目录（`C:\Windows\System32`）
9. 16 位 Windows 系统目录（`C:\Windows\System`）
10. Windows 系统目录（`C:\Windows`）
11. 进程当前目录
12. 环境变量 PATH

> - `LoadLibrary`
> - `LoadLibraryEx`：可设置修改标准搜索路径、仅加载资源数据，通常对同一 dll 不能与 `LoadLibrary` 混用
> - `GetProcAddress`
> - `FreeLibrary`
> - `FreeLibraryAndExitThread`
> - `EXTERN_C IMAGE_DOS_HEADER __ImageBase;`：由链接器创建的变量，位于该模块的基地址
> - `EnumProcessModules`
> - `GetModuleHandle`：不递增引用计数
> - `GetModuleHandleEx`：默认递增引用计数
> - `GetModuleBaseName`
> - `GetModuleFileName`
> - `QueryFullProcessImageName`：用来获取其它进程的 exe 文件路径更加高效且准确

### 虚拟内存

> 参考 [Memory Management](https://learn.microsoft.com/en-us/windows/win32/memory/memory-management)

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

  1. CPU 中的内存管理单元（MMU）负责根据页表基址寄存器（PTBR）存储的页表基址从翻译后备缓冲器（TLB）获取虚拟地址对应的页表表项（PTE）从而翻译为物理地址

  2. 操作系统为每个进程维护一个多级页表，最后一级页表表项记录虚拟地址对应的物理地址，其它级页表表项记录下一级页表基地址，通常只有一级页表常驻内存

  3. 根据页表索引到页面的物理地址后，有以下两种情况
     - 对应页面已经在内存中，说明该页面在此之前已经被本进程或其它进程访问过了
     - 对应页面没有在内存中，说明该页面可能是第一次访问，或之前被冲刷到后备存储器了
       1. 触发缺页异常，控制流转到操作系统异常处理程序
       2. 利用文件系统驱动，将对应的页面从其后备存储器中加载到内存
       3. 当内存中无空闲页面时，根据某种缓存驱逐策略来选择使用页面，若为脏页则先将其冲刷到其后备存储器再使用

- 工作集 (Working Set)

  - 每个进程维护一个工作集，管理驻留在内存中的页面
  - 最小工作集大小默认 50 个页面
  - 最大工作集大小默认 345 个页面

- 内存池 (Memory Pool)
  - 内核使用的内存分为两种
  - paged pool: 可交换出物理内存。单处理器系统有 3 个，多处理器系统有 5 个
  - nonpaged pool: 驻留在物理内存中

> - `GetSystemInfo`：CPU 硬件信息
> - `GetPerformanceInfo`: 系统运行时性能信息
> - `GlobalMemoryStatusEx`: 系统内存使用情况
> - `GetProcessMemoryInfo`: 进程内存使用情况
> - `VirtualQuery`
> - `VirtualAlloc`：控制内存页面状态，申请[大页内存](https://learn.microsoft.com/en-us/windows/win32/memory/large-page-support)
> - `VirtualFree`
> - `VirtualProtect`
> - `CreateFile`
> - `CreateFileMapping`
> - `MapViewOfFile`
> - `HeapCreate`
> - `HeapDestroy`
> - `HeapAlloc`
> - `HeapFree`
> - `HeapReAlloc`
> - `GetProcessHeap`：每个进程有一个默认的线程安全的堆
> - `GetProcessHeaps`
> - `GetProcessWorkingSetSize`
> - `SetProcessWorkingSetSize`

### 异常处理

> 参考 [Error Handling](https://learn.microsoft.com/en-us/windows/win32/debug/error-handling)、[Structured Exception Handling](https://learn.microsoft.com/en-us/windows/win32/debug/structured-exception-handling)、[Windows Error Reporting](https://learn.microsoft.com/en-us/windows/win32/wer/windows-error-reporting)

- 返回码：几乎所有系统 API 的调用都会失败，通常返回特殊的值表示调用失败

  - `BOOL`: 返回 `FALSE` 表示失败
  - `HANDLE`: 返回 `NULL` 或 `INVALID_HANDLE_VALUE`（特别注意）
  - `LPVOID`: 返回 `NULL` 表示失败
  - `HRESULT`: 使用 `SUCCEEDED(hr)` 或 `FAILED(hr)` 来检测成功或失败

- 错误码：当系统 API 调用失败时（少数是在调用成功时）设置一个线程独立的[**错误码**](https://learn.microsoft.com/en-us/windows/win32/debug/system-error-codes)来表示失败（或成功）的原因

```cpp
std::string GetLastErrorAsString() {
    DWORD errorMessageID = ::GetLastError();
    if(errorMessageID == 0) {
        return std::string();
    }

    LPSTR messageBuffer = nullptr;
    size_t size = FormatMessageA(
        FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS,
        NULL, errorMessageID, MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), (LPSTR)&messageBuffer, 0,
        NULL);

    if (size) {
      std::string message(messageBuffer, size);
      LocalFree(messageBuffer);
    }

    return message;
}
```

- 错误模式：当进程发生严重错误时，系统默认会显示错误对话框，对话框会挂起进程（同步执行）

  - `SEM_FAILCRITICALERRORS`，不显示对话框转而将错误发送给进程，通常应该设置改标志
  - `SEM_NOALIGNMENTFAULTEXCEPT`，在支持的平台上自动修复内存对齐错误
  - `SEM_NOGPFAULTERRORBOX`，不显示 WER 对话框
  - `SEM_NOOPENFILEERRORBOX`，不显示当 `OpenFile` 传入 `OF_PROMPT` 标志且对应文件不存在时的对话框

- 结构化异常处理 (SEH)：使用 SEH 后编译器禁止在同一栈帧中构造 C++ 对象，**因为 SEH 的栈展开不会调用析构函数**

```cpp
/*
 * Termination-Handler Syntax
*/
__try {
    // 可以使用 __leave 提前离开 __try 块且避免非正常离开和性能处罚
} __finally {
    // 只要控制流离开 __try 块就会执行 __finally 块，包括栈展开，除非因线程终止而离开 __try 块
}

/*
 * Exception-Handler Syntax
*/
__try {
    // 当发生底层的硬件或软件错误时会触发异常，也可以调用 RaiseException 手动触发
}
// 过滤表达式结果可以是以下值之一：
// EXCEPTION_CONTINUE_EXECUTION 表示继续执行发生异常的指令，注意可能导致死循环
// EXCEPTION_EXECUTE_HANDLER 表示执行后续 __except 块
// EXCEPTION_CONTINUE_SEARCH 表示搜索外部 __except 块
__except (filter-expression) {
    // 当搜索到能够处理异常的 __except 块时，先进行栈展开 (unwind) 直到 __excpet 块所在栈帧，然后继续执行 __except 块
}
```

- 向量化异常处理 (VEH)

  - 在异常触发后且在首次通知调试器后调用
  - 返回 `EXCEPTION_CONTINUE_EXECUTION` 表示继续执行发生异常的指令，跳过后续 VEH
  - 返回 `EXCEPTION_CONTINUE_SEARCH` 表示执行后续 VEH，然后开始搜索 SEH

- 向量化继续处理 (VCH)

  - 在继续执行发生异常的指令之前调用
  - 返回 `EXCEPTION_CONTINUE_EXECUTION` 表示继续执行发生异常的指令，跳过后续 VCH
  - 返回 `EXCEPTION_CONTINUE_SEARCH` 表示执行后续 VCH，然后从异常指令开始重新搜索 SEH

- 未处理异常

  - `UnhandledExceptionFilter` 作为最外部的全局 SEH 的过滤表达式
  - 其内部会调用 `SetUnhandledExceptionFilter` 设置的过滤函数
  - C++ 程序默认设置了未处理异常过滤函数，用来过滤 C++ 异常

- Window 错误报告 (WER)
  - Dumps: 需要设置 `HKEY_LOCAL_MACHINE\Software\Microsoft\Windows\Windows Error Reporting\LocalDumps`
  - Recovery: 需要调用 `RegisterApplicationRecoveryCallback`
  - MessageBox: 需要设置 `HKEY_LOCAL_MACHINE\Software\Microsoft\Windows\Windows Error Reporting\DontShowUI` 为 0，且 `GetErrorMode` 标志没有 `SEM_NOGPFAULTERRORBOX`
  - Restart: 需要调用`RegisterApplicationRestart`

![seh](./images/seh.png)

> - `GetLastError`
> - `SetLastError`
> - `FormatMessage`
> - `GetErrorMode`
> - `SetErrorMode`
> - `GetThreadErrorMode`
> - `SetThreadErrorMode`
> - `_controlfp_s`: 默认系统关闭所有浮点异常，因此计算结果可以是 NAN 或 INFINITY 而不是异常。
> - `_clearfp`: 必须在浮点异常处理块中调用该函数来获取并清除浮点异常标志
> - `RaiseException`
> - `GetExceptionCode`: 仅可在过滤表达式和 `__except` 块中调用
> - `GetExceptionInformation`: 仅可在过滤表达式中调用，因为执行 `__except` 块时异常栈帧已被销毁
> - `AbnormalTermination`: 仅可在 `__finally` 块中调用
> - `AddVectoredExceptionHandler`
> - `RemoveVectoredExceptionHandler`
> - `AddVectoredContinueHandler`
> - `RemoveVectoredContinueHandler`
> - `UnhandledExceptionFilter`
> - `SetUnhandledExceptionFilter`
> - `RegisterApplicationRecoveryCallback`
> - `RegisterApplicationRestart`

## IO 系统

操作系统提供了“内核对象”的机制，简化了程序对系统中“IO 资源”的使用，如设备、文件、网络、IPC 等。

### 句柄表

每个进程维护一张句柄表用于访问内核对象，内核对象句柄仅在进程内有效

- 索引：`HANDLE`
  - 0 保留作为无效句柄
  - 1、2、3 保留用作标准输入输出
- 内核对象指针
  - 名字
  - 引用计数
  - 安全描述符
  - 其他...
- 访问掩码：这个句柄的访问权限掩码
- 属性标志：如 Protect, Inherit 等

> - `GetHandleInformation`
> - `SetHandleInformation`
> - `DuplicateHandle`
> - `CloseHandle`

### 命名空间

大多数内核对象都可以通过名字访问，所有对象都实际存在于底层 NT 命名空间内，但不同类型的对象名字处于不同的子命名空间内。可以使用 [WinObj](https://learn.microsoft.com/en-us/sysinternals/downloads/winobj) 查看详细内容。

![winobj](./images/winobj2.png)

- 与同步相关的内核对象，如 `Event`, `Mutex`, `Semaphore` 等

  - 不同会话中的对象默认位于不同命名空间
    - 服务会话（session 0）中的对象名字位于 `\BaseNamedObjects`
    - 终端会话（如 session 1）中的对象名字位于 `\Sessions\1\BaseNamedObjects`
  - 这些命名空间内都存在两个符号链接
    - `Global` 链接到 `\BaseNamedObjects`，用于跨会话共享
    - `Local` 链接到 `\Session\1\BaseNamedObjects`，用于不跨会话共享

---

![winobj](./images/winobj.png)

- 与文件相关的内核对象
  - 文件路径名均位于 `\GLOBAL??`
  - 其内包含
    - `C:` 链接到 `\Device\HarddiskVolume3`
    - `GLOBALROOT` 链接到 `\`

### 访问控制

#### 隔离性

> 参考 [Window Stations and Desktops](https://learn.microsoft.com/en-us/windows/win32/winstation/window-stations-and-desktops)

![session](./images/session.png)

- Session：由单个用户登录会话产生的所有进程和内核对象组成

  - Session 0 由系统创建专门用于运行服务 (Services)
  - 不同会话的部分内核对象的默认命名空间不同

- Window Station：由一个剪切板、一张原子表和若干 desktop 组成

  - WinSta0 由系统创建，唯一能与用户终端设备交互的 Window Station
  - 同一 Window Station 内仅允许一个 Desktop 访问用户终端设备

- Desktop：由若干 windows, menus, hooks 组成

  - Winlogon 用于用户登录和 UAC 授权
  - Default 用于用户应用程序
  - ScreenSaver 用于屏保
  - windows, menu, hooks 仅能在同一 Desktop 内部访问

#### 访问控制模型

> 参考
>
> - [Access Tokens](https://learn.microsoft.com/en-us/windows/win32/secauthz/access-tokens)
> - [Security Descriptors](https://learn.microsoft.com/en-us/windows/win32/secauthz/security-descriptors)
> - [Access Control Lists](https://learn.microsoft.com/en-us/windows/win32/secauthz/access-control-lists)
> - [Access Control Entries](https://learn.microsoft.com/en-us/windows/win32/secauthz/access-control-entries)
> - [Access Rights and Access Masks](https://learn.microsoft.com/en-us/windows/win32/secauthz/access-rights-and-access-masks)
> - [Security Identifiers](https://learn.microsoft.com/en-us/windows/win32/secauthz/security-identifiers)
> - [Privileges](https://learn.microsoft.com/en-us/windows/win32/secauthz/privileges)
> - [Client Impersonation](https://learn.microsoft.com/en-us/windows/win32/secauthz/client-impersonation)

- 访问令牌 (Access Token)

  - user SID
  - group SIDs
  - logon SID
  - privileges LUIDs
  - owner SID
  - primary group SID
  - integrity SIDs
  - default DACL
  - restricting SIDs
  - whether impersonation
  - others...

- 安全描述符 (Security Descriptor)

  - owner SID
  - primary group SID
  - DACL
  - SACL
  - others...

- 访问控制列表 (Access Control Lists)

  - 访问控制表项 (Access Control Entries)
    - trustee SID
    - type flag
    - inherit flags
    - access mask
      - Generic Access Rights：被映射到 Object-specific Access Right
      - SACL Access Right：访问对象 SACL 的权限
      - Standard Access Rights：用于控制对对象本身的操作
        - `DELETE`
        - `READ_CONTROL`
        - `SYNCHRONIZE`
        - `WRITE_DAC`
        - `WRITE_OWNER`
      - Object-specific Access Right：针对不同类型对象的特殊操作的权限

![acess mask format](./images/access_mask_format_.png)

- 安全对象的访问控制流程
  1. 若没有 DACL，则允许所有访问
  2. 若存在 DACL，则使用第一个匹配的 ACE 访问控制，若没有匹配的 ACE 则拒绝所有访问
  3. 根据 SACL 是否记录该访问尝试

#### 强制可信控制

> 参考
>
> - [Mandatory Integrity Control](https://learn.microsoft.com/en-us/windows/win32/secauthz/mandatory-integrity-control)
> - [Windows Integrity Mechanism Design](<https://learn.microsoft.com/en-us/previous-versions/dotnet/articles/bb625963(v=msdn.10)>)
> - [Allow UIAccess](https://learn.microsoft.com/en-us/previous-versions/windows/it-pro/windows-10/security/threat-protection/security-policy-settings/user-account-control-allow-uiaccess-applications-to-prompt-for-elevation-without-using-the-secure-desktop)

- 可信级别（integrity level）

  - system: 通常为系统服务
  - high: 通常为管理员权限进程
  - medium: 通常为标准用户权限进程
  - low: 通常显式设置了 exe 文件的 security descriptors 或进程的 access token

- access token 中的 integrity SIDs 表示可信级别

- security descriptors 中的 SACL 中存储可信级别和强制策略

- ACM: 可信级别校验发生在 DACL 校验之前，默认拒绝较低可信级别的写入访问

- Privileges: 某些系统特权仅允许高可信级别进程运行

- UIPI：限制低可信级别对高可信级别的访问机制
  - 验证窗口句柄
  - 发送窗口消息（API 调用返回成功，消息被静默丢弃）
  - 除非应用程序具有 UIAccess 标志且满足以下条件，则允许对高可信级别进程驱动 UI 自动化
    - 程序具有可信的数字签名
    - 程序安装在 `%ProgramFiles%` 或 `%WinDir%` (某些标准用户可写的子目录除外) 目录下
  - Hook
  - DLL 注入

#### 用户访问控制

用户访问控制 (UAC) 让标准用户不用重新登录就能使用管理员权限

- Administrator Broker Model：使用 `ShellExecute` 创建管理员权限的新进程
- Operating System Service Model：使用 IPC 与 Service 通信
- Elevated Task Model：使用 Task Scheduler 服务运行应用程序

### 注册表

注册表是 Windows 用来存储配置的层次结构数据库(B-Tree)，支持事务。

- Key: Subkey 和 Value 的集合
  - Name: 不可包含 `\`，忽略大小写
- Value: 包含数据的记录

  - Name: 可以包含 `\`，忽略大小写
  - Type
  - Data

- Computer

  - **HKEY_LOCAL_MACHINE**: 系统全局配置

    - **Software**: 软件配置，32 位程序访问该 Key 会被重定向到 **WOW6432Node**

      - **CLASS**: 文件扩展名配置

    - **WOW6432Node**: 具体见 [Registry Keys Affected by WOW64](https://learn.microsoft.com/en-us/windows/win32/winprog64/shared-registry-keys)

  - **HKEY_CURRENT_USER**: 当前用户配置（漫游）

    - **Software**: 软件配置，不会重定向 32 位程序

      - **CLASS**: 文件扩展名配置

    - **Environment**: 用户环境变量

  - **HKEY_CURRENT_USER_LOCAL_SETTINGS**: 当前用户配置（不漫游）
  - **HKEY_USERS**: 所有用户配置实际所在

> - `RegEnumKeyEx`
> - `RegQueryInfoKey`
> - `RegGetKeySecurity`
> - `RegNotifyChangeKeyValue`
> - `RegOpenKeyExW`
> - `RegOpenCurrentUser`：打开当前模拟用户的 **HKEY_CURRENT_USER**
> - `RegOpenUserClassesRoot`：打开当前模拟用户的 **HKEY_CURRENT_USER\Software\Classes** 和 **HKEY_LOCAL_MACHINE\Software\Classes** 合并后的视图，注意需要模拟用户配置已被加载（比如用户登录），使用 `LoadUserProfile` 手动加载
> - `RegCreateKeyEx`
> - `RegDeleteKey`
> - `RegCloseKey`
> - `RegFlushKey`
> - `RegQueryValueEx`
> - `RegQueryMultipleValues`
> - `RegSetValueEx`
> - `RegDeleteValue`
> - `RegSaveKeyEx`：将注册表条目保存到文件，注意这不同于 `.reg` 文件（用 `regedit.exe` 来导入）
> - `RegLoadKey`：不覆盖已存在的 key，可以用 `RegUnLoadKey` 还原
> - `RegReplaceKey`：覆盖已存在的 key
> - `RegRestoreKey`

### 文件系统

> 参考 [File Management](https://learn.microsoft.com/en-us/windows/win32/fileio/file-management)

NTFS 支持事务

#### 磁盘

| 特性         | MBR 分区表                           | GPT 分区表      |
| ------------ | ------------------------------------ | --------------- |
| 适用固件     | BIOS                                 | UEFI            |
| 分区大小     | 最大支持近 2TB                       | 最大支持近 18EB |
| 主分区       | 最多只能有 3 个主分区与 1 个扩展分区 | 全部是主分区    |
| 引导加载器   | 在 MBR 头部以及主分区头部            | 在 ESP 中       |
| 分区表健壮性 | 无校验、 无备份                      | 有校验、有备份  |

![mbr](./images/MBR.png)

![gpt](./images/GPT.jpg)

> - `FindFirstVolume`
> - `FindNextVolume`
> - `FindVolumeClose`
> - `GetVolumeInformation`
> - `GetDiskFreeSpaceEx`
> - `IDiskQuotaControl`
> - `IEnumDiskQuotaUsers`

#### 文件

- 文件：一个文件由若干个文件流组成，文件流包含了文件的元数据和数据，引用文件流 `file.txt:strm:$DATA`，其中 `$DATA` 为流类型（可忽略），可能需要添加路径 `./` 以防止文件名被解析为盘符

  - 唯一对象标识
  - 安全描述符
  - 重解析点
  - 文件名和属性
    - 文件名
    - 日期时间（创建、访问、修改）
    - 文件大小（使用、分配）
    - 只读
    - 隐藏
    - 压缩
    - 加密
    - 可被备份
    - 可被索引
    - ...
  - 文件数据
  - ...

- 链接

  - 硬链接
    - 即目录项，每个硬链接指向单独的文件流记录文件名和属性
    - 不可跨文件系统
  - 软连接 (Junctions)
    - 利用重解析点，仅可指向目录
    - 可跨本地文件系统，不可跨网络文件系统
  - 符号链接
    - 利用重解析点
    - 可跨网络文件系统

> - `FindFirstFile`
> - `FindNextFile`
> - `FindFirstStream`
> - `FindNextStream`
> - `FindClose`
> - `GetFileInformationByHandle`
> - `SetFileInformationByHandle`
> - `GetFileAttributes`
> - `SetFileAttributes`
> - `GetFileType`
> - `GetBinaryType`
> - `GetFileTime`
> - `SetFileTime`
> - `GetFileSize`
> - `CreateFile`
> - `DeleteFile`：直到最后一个文件句柄被关闭才真正删除文件，而在此之前文件就无法被打开了
> - `CopyFile`
> - `CopyFileExA`：可以获取拷贝进度
> - `MoveFile`：目标路径不能已存在
> - `MoveFileEx`：可以获取移动进度
> - `ReplaceFile`：目标路径必须已存在，仅覆盖数据文件流和少量属性
> - `CreateDirectory`
> - `RemoveDirectory`
> - `ReadDirectoryChangesW`
> - `CreateHardLink`
> - `CreateSymbolicLink`

#### 路径

- 文件名

  - 不能包含特殊字符：`<` `>` `:` `"` `/` `\` `|` `?` `*` `NUL` 控制字符等
  - 不能使用保留名称：CON, PRN, AUX, NUL, COM0, LPT0 等，即使如 NUL.txt 带扩展名也不行
  - 不能以 `.` 或 `SPC` 结尾
  - 默认忽略大小写

- 路径名

  - 路径是以 `\` 分隔组件名（卷、目录、文件）的字符串

  - 相对路径：如 `file.txt` `.\file.txt` `..\file.txt` `\file.txt` `C:file.txt`

    - 会根据当前卷和当前目录，或指定卷的当前目录扩展成完全限定路径
    - 限制最大长度为 MAX_PATH (260)，包含末尾 `NUL`
    - 可以使用 `.` `..` `/`

  - 完全限定路径：如 `C:\file.txt`

    - 限制最大长度为 MAX_PATH (260)，包含末尾 `NUL`
    - 可以使用 `.` `..` `/`

  - Win32 文件命名空间：如 `\\?\C:\file.txt`

    - 限制最大长度大概为 32767，包含末尾 `NUL`，但单个文件名不能超过 255
    - 不能使用 `.` `..` `/`，且必须为完全限定路径

  - Win32 设备命名空间：如 `\\.\PhysicalDrive0`

    - 直接指定 NT 命名空间中的 `GLOBAL??` 里的设备对象

> - `GetLongPathName`
> - `GetShortPathName`
> - `GetFullPathName`
> - `GetTempPath2`：返回路径长度最大 MAX_PATH+1，不会校验路径是否已存在
> - `GetTempFileName`：自动或手动创建唯一文件名 `<prefix><uuuu>.TMP`

#### 读写

- 以下标志会影响缓存行为

  - `FILE_FLAG_RANDOM_ACCESS`
  - `FILE_FLAG_SEQUENTIAL_SCAN`
  - `FILE_FLAG_NO_BUFFERING`
  - `FILE_FLAG_WRITE_THROUGH`
  - `FILE_ATTRIBUTE_TEMPORARY`

- 创建文件对象时，会包含一个数据文件流的偏移量，称为文件指针，每次调用读写接口都会自动更新指针位置。

> - `ReadFile`
> - `ReadFileEx`
> - `WriteFile`
> - `WriteFileEx`
> - `FlushFileBuffers`
> - `SetFilePointer`
> - `SetEndOfFile`

### IPC 机制

> 参考 [Interprocess communications](https://learn.microsoft.com/en-us/windows/win32/ipc/interprocess-communications)

#### 套接字

- Server

  1. `WSAStartup`
  2. `WSASocket`
  3. `bind`
  4. `listen` (tcp only)
  5. `AcceptEx` (tcp only)
  6. `WSASend`/`WSARecv`
  7. `LPFN_DISCONNECTEX` (tcp only)
  8. `closesocket`
  9. `WSACleanup`

- Client

  1. `WSAStartup`
  2. `WSASocket`
  3. `GetAddrInfoEx`
  4. `LPFN_CONNECTEX` (implicit `bind`, tcp only)
  5. `FreeAddrInfoEx`
  6. `WSASend`/`WSARecv`
  7. `LPFN_DISCONNECTEX` (tcp only)
  8. `WSARecv`
  9. `closesocket`
  10. `WSACleanup`

#### 管道

- 匿名管道：
  - 单向通讯
  - 创建后只能通过子进程继承的方式获取 handle
  - 见示例 [Creating a Child Process with Redirected Input and Output](https://learn.microsoft.com/en-us/windows/win32/procthread/creating-a-child-process-with-redirected-input-and-output)

> - `CreatePipe`
> - `GetStdHandle`
> - `SetStdHandle`

- 命名管道：
  - 支持单向和双向
  - 创建后可通过管道名字的方式获取 handle，如 `\\.\pipe\PipeName`
  - 仅当服务端存在空闲的命名管道实例，客户端才能打开它
  - 见示例 [Named Pipe Server](https://learn.microsoft.com/en-us/windows/win32/ipc/named-pipe-server-using-overlapped-i-o) 和 [Named Pipe Client](https://learn.microsoft.com/en-us/windows/win32/ipc/named-pipe-client)

> - `CreateNamedPipe`：服务端创建命名管道
> - `ConnectNamedPipe`：服务端等待连接，客户端可能在此之前就打开管道了，比如在 `CreateNamedPipe` 或 `DisconnectNamedPipe` 之后
> - `DisconnectNamedPipe`：服务端关闭连接，使客户端句柄失效（客户端仍需要调用 CloseHandle），同时丢弃缓冲区中的数据
> - `CreateFile`：客户端连接服务端命名管道
> - `WaitNamedPipe`：客户端等待服务端空闲管道

#### 数据拷贝

> 参考 [Why can't I PostMessage the WM_COPYDATA message](https://devblogs.microsoft.com/oldnewthing/20110916-00/?p=9623)

```cpp
auto data = COPYDATASTRUCT {
  .dwData = data_type,
  .cbData = data_size,
  .lpData = data_buf,
};

// 系统需要确定何时释放缓冲区，所以 WM_COPYDATA 只能使用同步发送消息，如 SendMessage, SendMessageTimeout 等
SendMessage(target_hwnd, WM_COPYDATA, hwnd, &data);
```

#### 共享内存

> 参考 [Creating Named Shared Memory](https://learn.microsoft.com/en-us/windows/win32/memory/creating-named-shared-memory)
>
> - `CreateFileMapping`
> - `OpenFileMapping`
> - `MapViewOfFile`
> - `UnmapViewOfFile`

#### 总结

- 套接字

  - 每个连接需要单独的 2 对读写缓冲区，缓冲可以被复用
  - 完善的缓冲区管理和连接管理，支持流传输
  - 服务端监听网络端口需要设置防火墙规则
  - **适用场景：跨机器的网络通讯**

- 双向命名管道

  - 每个连接需要单独的 1 对读写缓冲区，缓冲可以被复用
  - 有缓冲区管理，但需要自己完善连接管理（特别是断开连接会丢弃缓冲区数据），支持流传输
  - **适用场景：本机的进程间通讯**

- 数据拷贝

  - 每次发送数据都需要分配缓冲区，且只能同步发送
  - 有缓冲区管理，没有连接管理，不支持流传输
  - **适用场景：UI 同步逻辑通讯**

- 共享内存
  - 可以直接在共享内存中**构造**消息对象，单次通讯可省去一次拷贝（构造期间需要加锁，通常来说构造数据比拷贝更慢）
  - 可以直接在共享内存中**访问并处理**消息对象，单次通讯可省去一次拷贝（处理期间需要加锁，通常来说处理数据比拷贝更慢）
  - 没有缓冲区管理，没有连接管理，没有流传输
  - **适用场景：传输 Raw Data（数据量大且处理快）**

### 异步 IO

> 参考
>
> - [Synchronization and Overlapped Input and Output](https://learn.microsoft.com/en-us/windows/win32/sync/synchronization-and-overlapped-input-and-output)
> - [Asynchronous Procedure Calls](https://learn.microsoft.com/en-us/windows/win32/sync/asynchronous-procedure-calls)
> - [I/O Completion Ports](https://learn.microsoft.com/en-us/windows/win32/fileio/i-o-completion-ports)

Windows 支持三种异步 IO 机制：

- Overlapped IO：利用可同步对象
- APC：利用 APC 任务队列
- IOCP：利用 IOCP 消息队列

其中前两者可以使用同一个多路复用接口 `MsgWaitForMultipleObjectsEx` 一起监听，而 IOCP 则不行。

- 使用流程

  1. 创建 IOCP `CreateIoCompletionPort` (iocp only)
  2. **创建 HANDLE `CreateFile` 使用 `FILE_FLAG_OVERLAPPED` 标志**
  3. 关联 HANDLE `CreateIoCompletionPort` (iocp only)
  4. **调用异步 IO 接口**
  5. **等待 IO 事件完成** `GetQueuedCompletionStatus`/`WaitForMultipleObjectsEx`
  6. **IO 事件完成，唤醒等待线程**
     - Overlapped IO
       - 唤醒等待对应 Event 对象的线程
     - APC
       - 唤醒 Alertable 状态的线程并执行对应回调函数
     - IOCP
       - FIFO 顺序通知等待线程
       - IOCP 设置有最大并发数，超过后阻塞后续等待线程
       - 当线程处于其他阻塞状态时（如 `SuspendThread` 或 `WaitForMultipleObjectsEx`），系统会通知其他线程，这时如果阻塞线程被唤醒，则 IOCP 关联线程会超过最大并发数

- Overlapped-IO/IOCP 支持的异步 IO 有：

  - `ReadFile`
  - `WriteFile`
  - `AcceptEx`
  - `LPFN_CONNECTEX`
  - `LPFN_DISCONNECTEX`
  - `WSASend`
  - `WSASendTo`
  - `WSASendMsg`
  - `WSARecv`
  - `WSARecvFrom`
  - `LPFN_WSARECVMSG`
  - `ConnectNamedPipe`
  - `TransactNamedPipe`
  - `WaitCommEvent`
  - `LockFileEx`
  - `DeviceIoControl`
  - `ReadDirectoryChangesW`
  - `PostQueuedCompletionStatus` (iocp only)

- APC 支持的异步 IO 有：

  - `ReadFileEx`
  - `WriteFileEx`
  - `SetWaitableTimer`
  - `SetWaitableTimerEx`
  - `QueueUserAPC`

- 取消异步 IO

  - `CancelIo`
  - `CancelIoEx`

- 线程池
  - `BindIoCompletionCallback`
  - 详细内容见 [Thread Pool API](https://learn.microsoft.com/en-us/windows/win32/procthread/thread-pool-api)

## 窗口系统

### 桌面环境

> 参考
>
> - [Window Stations and Desktops](https://learn.microsoft.com/en-us/windows/win32/winstation/window-stations-and-desktops)

- Window Station

  - Desktops

    - Monitors

      - Desktop Window
      - Taskbar

      - Application Window

        - Non-client Area
        - Client Area

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

## 其他

### 头文件宏

- Windows SDK 最低支持版本
  - `_WIN32_WINNT` 指定大版本
  - `NTDDI_VERSION` 指定更细化的版本，需要同时指定 `_WIN32_WINNT`
- 减小头文件内容以加速编译
  - `WIN32_LEAN_AND_MEAN`
  - `NOCOMM`

### 字符集

> 参考
>
> - [Character Sets](https://learn.microsoft.com/en-us/windows/win32/intl/character-sets)
> - [Code Page Identifiers](https://learn.microsoft.com/en-us/windows/win32/intl/code-page-identifiers)
> - [Unicode in the Windows API](https://learn.microsoft.com/en-us/windows/win32/intl/unicode-in-the-windows-api)
>
> 因为 UTF-8 诞生之前，Windows 就应用了 UTF-16 作为 Unicode 编码，故 UTF-8 仅作为 Code Page 中的一个兼容选项。虽然绝大多数 API 交互使用 UTF-16，但系统中的文件数据都应该存储为 UTF-8。

Windows API 通常有三种格式：

- `CreateFile` 宏，根据不同编译参数扩展成以下二者之一
- `CreateFileW` Unicode 版本，使用 UTF-16
- `CreateFileA` ANSI 版本，使用 Windows Code Page （又名 ANSI Code Pages）
  - 单字节字符集，如 `OEM United States`、`IBM EBCDIC International`
  - 多字节字符集，如 `Chinese Simplified (GB2312)`、`utf-8`

字符串编码转换：

- `TCHAR`
- `TEXT`
- `WideCharToMultiByte`
- `MultiByteToWideChar`
- 系统自动转换窗口消息中的字符串
