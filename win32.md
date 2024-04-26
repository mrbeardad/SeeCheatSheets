# Win32

- [Win32](#win32)
  - [进程系统](#进程系统)
    - [进程加载](#进程加载)
    - [动态链接](#动态链接)
    - [虚拟内存](#虚拟内存)
    - [线程管理](#线程管理)
    - [进程管理](#进程管理)
  - [资源系统](#资源系统)
    - [内核对象](#内核对象)
    - [用户对象](#用户对象)
    - [GDI 对象](#gdi-对象)
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

操作系统提供了“进程”这个概念，简化了对计算机 CPU 和内存的使用，以实现“资源计算”。

### 进程加载

**进程的生命周期**：

1. `CreateProcess`
2. 创建进程和线程的内核对象
3. 创建进程的虚拟地址空间
4. 加载映像文件（exe 和 dll）
5. 调用 c/c++ runtime library 入口，负责初始化命令行参数、环境变量、全局变量和内存分配等
6. 调用用户程序入口

   - `main`/`wmain` for **SUBSYSTEM:CONSOLE**, 默认自动创建控制台窗口或继承父进程控制台窗口来执行程序
   - `WinMain`/`wWinMain` for **SUBSYSTEM:WINDOWS**

   ```cpp
   int main(int argc, char* argv[], char* env[]);

   int APIENTRY WinMain(HINSTANCE hInst, HINSTANCE hPrevInstance, PSTR lpCmdLine, int nShowCmd);
   ```

7. 进程终止
   - 主线程入口函数返回，然后由 c/c++ runtime library 做清理（如`std::atexit`、static 变量的析构函数等）后再退出
   - 直接调用了 `ExitProcess` 或 `TerminateProcess` 函数，没有 c/c++ runtime library 清理
   - 进程执行出错，并弹出错误窗口（可调用 `SetErrorMode` 设置弹窗）
8. 资源释放
   - 终止进程中所有线程
   - 释放进程的用户对象和 GDI 对象，并关闭进程持有的内核对象
   - 触发线程和进程的内核对象

**线程的生命周期**：

1. 调用 c/c++ runtime library 线程入口，负责初始化 thread_local 变量等
2. `CreateThread`
3. 分配线程栈
4. 执行线程函数
5. 线程终止
   - 线程函数返回，并由 c/c++ runtime library 做清理（如 thread_local 变量的析构函数）
   - 直接调用 `ExitThread` 或 `TerminateThread` 函数，没有 c/c++ runtime library 清理
   - 进程终止（见上）
6. 触发线程的内核对象

**exe 搜索路径**：（仅适用于无路径文件名）

1. 进程 exe 文件所在目录
2. 进程当前目录
3. Windows\System32
4. Windows\
5. PATH 环境变量

> 相关接口：
>
> - `CreateProcess`: 创建进程、句柄继承、错误模式继承、控制台继承、初始窗口命令、环境变量、当前目录等
> - `CreateThread`
> - `GetCommandLine`
> - `CommandLineToArgv`
> - `GetEnvironmentVariable`
> - `SetEnvironmentVariable`
> - `ExpandEnvironmentStrings`
> - `GetCurrentDirectory`
> - `SetCurrentDirectory`
> - `GetFullPathName`
> - `MAX_PATH`: 260 个字符包含 `D:\some 256-character path string<NUL>`

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
6. Windows 11, version 21H2 (10.0; Build 22000), and later. The package dependency graph of the process. This is the application's package plus any dependencies specified as `<PackageDependency>` in the `<Dependencies>` section of the application's package manifest. Dependencies are searched in the order they appear in the manifest.
7. The folder from which the application loaded.
8. The system folder. Use the GetSystemDirectory function to retrieve the path of this folder.
9. The 16-bit system folder. There's no function that obtains the path of this folder, but it is searched.
10. The Windows folder. Use the GetWindowsDirectory function to get the path of this folder.
11. The current folder.
12. The directories that are listed in the PATH environment variable. This doesn't include the per-application path specified by the App Paths registry key. The App Paths key isn't used when computing the DLL search path.

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
> - `FreeLibraryAndExitThread`
> - `__ImageBase`
> - `GetModuleHandle`: 不递增引用计数
> - `GetModuleHandleEx`: 默认递增引用计数，可设置 dll 直到进程终止前绝不卸载
> - `GetModuleFileName`

### 虚拟内存

<!-- TODO: 虚拟内存 -->

### 线程管理

|                               | IDLE_PRIORITY_CLASS | BELOW_NORMAL_PRIORITY_CLASS | NORMAL_PRIORITY_CLASS | ABOVE_NORMAL_PRIORITY_CLASS | HIGH_PRIORITY_CLASS | REALTIME_PRIORITY_CLASS |
| ----------------------------- | ------------------- | --------------------------- | --------------------- | --------------------------- | ------------------- | ----------------------- |
| THREAD_PRIORITY_IDLE          | 1                   | 1                           | 1                     | 1                           | 1                   | 16                      |
| THREAD_PRIORITY_LOWEST        | 2                   | 4                           | 6                     | 8                           | 11                  | 22                      |
| THREAD_PRIORITY_BELOW_NORMAL  | 3                   | 5                           | 7                     | 9                           | 12                  | 23                      |
| THREAD_PRIORITY_NORMAL        | 4                   | 6                           | 8                     | 10                          | 13                  | 24                      |
| THREAD_PRIORITY_ABOVE_NORMAL  | 5                   | 7                           | 9                     | 11                          | 14                  | 25                      |
| THREAD_PRIORITY_HIGHEST       | 6                   | 8                           | 10                    | 12                          | 15                  | 26                      |
| THREAD_PRIORITY_TIME_CRITICAL | 15                  | 15                          | 15                    | 15                          | 15                  | 31                      |

- 线程是调度的基本单位
- 内核线程与用户线程
- 抢占式调度：只要存在高优先级的线程处于可调度状态，就会先运行高优先级线程
- 动态提升线程优先级：
  - 接受新消息
  - 前台进程中的线程
  - 长时间处于饥饿状态的低优先级线程
- CPU 关联性

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

### 进程管理

- 作业（内核对象）：

  - 限制一组进程对一些资源的访问，比如 CPU 的使用、UI 限制、安全限制等
  - 将进程移入作业后不可移出
  - 子进程默认也在作业中
  - 可以终止作业中所有进程
  - 可以监听作业中进程的状态

- 会话
<!-- TODO: 会话 -->

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

### 内核对象

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

### 用户对象

### GDI 对象

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
