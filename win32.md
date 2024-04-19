# Win32

- [Win32](#win32)
  - [系统启动](#系统启动)
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
    - [绘制原理](#绘制原理)
    - [系统结构](#系统结构)
    - [应用窗口](#应用窗口)
      - [样式](#样式)
      - [关系](#关系)
      - [状态](#状态)
      - [通用流程](#通用流程)
    - [消息循环](#消息循环)
      - [生命周期](#生命周期)
      - [用户输入](#用户输入)
    - [实现细节](#实现细节)
      - [DPI](#dpi)
      - [Color](#color)

## 系统启动

1. UEFI
2. Boot Loader
3. Kernel
4. Services
5. Login

## 进程系统

### 进程加载

exe 执行流程：

1. 创建进程对象和线程对象
2. 加载可执行文件(.exe)和动态库(.dll)
3. 调用 c/c++ runtime 入口，负责初始化命令行参数、环境变量、全局变量和内存分配器等
4. 调用用户程序入口

   - `main`/`wmain` for SUBSYSTEM:CONSOLE, 默认自动创建终端窗口来执行程序
   - `WinMain`/`wWinMain` for SUBSYSTEM:WINDOWS

   ```cpp
   int main(int argc, char* argv[], char* env[]);

   int APIENTRY WinMain(HINSTANCE hInst, HINSTANCE hPrevInstance, PSTR lpCmdLine, int nShowCmd);
   ```

exe 搜索路径：

1. 进程 exe 文件所在目录
2. 进程当前目录
3. Windows 系统目录
4. Windows 目录
5. PATH 环境变量

> 相关接口：
>
> - `CreateProcess`
> - `GetCommandLine`
> - `CommandLineToArgv`
> - `GetEnvironmentVariable`
> - `SetEnvironmentVariable`
> - `ExpandEnvironmentStrings`

### 动态链接

- 对于进程中加载的每个模块(exe 和 dll)，系统都会为其维护一个引用计数
- `AddLibrary`和`FreeLibrary`会修改引用计数
- `GetModuleHandle`可获取进程已加载的模块的基地址 (`HINSTANCE`和`HMODULE`)
- DllMain 的执行会加锁

- DLL_PROCESS_ATTACH
- DLL_PROCESS_DETACH
- DLL_THREAD_ATTACH
- DLL_THREAD_DETACH

### 虚拟内存

### 线程管理

### 进程管理

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

- `GetHandleInformation`
- `SetHandleInformation`
- `DuplicateHandle`

### 用户对象

### GDI 对象

## 窗口系统

### 绘制原理

[Windows Graphics Overview](https://learn.microsoft.com/en-us/windows/win32/learnwin32/overview-of-the-windows-graphics-architecture)

### 系统结构

- Window Station
- Desktop
- Monitor
- Wallpaper
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

- Child

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

- Layerd Windows

  - `WS_EX_LAYERED`
  - 一般用于需要复杂形状、动画形状、alpha 混合的窗口
  - 在调用如下两者前不会显示
  - `SetLayeredWindowAttributes`：调用后无法调用`UpdateLayeredWindow`，直到清除`WS_EX_LAYERED`
  - `UpdateLayeredWindow`：应该主要在应用程序必须直接提供分层窗口的形状和内容时使用
  - `WS_EX_TRANSPARENT`: 整个窗口透过鼠标事件，默认仅 0 alpha 的像素透过鼠标事件

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

#### 通用流程

1. RegisterClassEx：exe 注册的 Class 在退出后销毁，dll 注册的样式需要手动销毁，class 由 classname 和 hinstance 唯一确定
2. CreateWindowEx

   ```cpp
   LONG_PTR GetWindowLongPtrW(
   [in] HWND hWnd,
   [in] int  nIndex
   );
   ```

3. ShowWindow
4. GetMessage
5. DispatchMessage
6. WindowProc

```cpp
int WINAPI wWinMain(
    HINSTANCE hInstance,
    HINSTANCE hPrevInstance,
    PWSTR pCmdLine,
    int nCmdShow
  );
    switch (uMsg)
    {
    case WM_CLOSE:
        DestroyWindow(hWindow);
        break;
    case WM_DESTROY:
        PostQuitMessage(0);
        break;
    default:
        return DefWindowProc(hWindow, uMsg, wParam, lParam);
    }
```

### 消息循环

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

### 实现细节

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
