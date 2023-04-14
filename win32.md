# Win32

- [Win32](#win32)
  - [窗口](#窗口)
    - [窗口区域](#窗口区域)
    - [窗口类型](#窗口类型)
    - [窗口状态](#窗口状态)
  - [流程](#流程)
  - [图形界面绘制原理](#图形界面绘制原理)

## 窗口

### 窗口区域

Application Window:

- Nonclient Areas
  - `WS_CAPTION`: title bar
  - `WS_THICKFRAME`: sizing border
  - `WS_SYSMENU`: window menu, application icon, close button
  - `WS_MINIMIZEBOX`: minimize button
  - `WS_MAXIMIZEBOX`: maximize and restore button
  - `WS_VSCROLL`: vertical scroll bar
  - `WS_HSCROLL`: horizontal scroll bar
- Client Area

### 窗口类型

- Overlapped Window
  - `WS_OVERLAPPED`: title bar
  - `WS_OVERLAPPEDWINDOW`: `WS_OVERLAPPED | WS_CAPTION | WS_SYSMENU | WS_THICKFRAME | WS_MINIMIZEBOX | WS_MAXIMIZEBOX`
  - 一般用作应用主窗口
- Pop-up Window
  - `WS_POPUP`: only client area
  - `WS_POPUPWINDOW`: `WS_POPUP | WS_BORDER | WS_SYSMENU`
  - 一般用作应用主窗口的临时辅助窗口
- Child Window
  - `WS_CHILD`: only client area
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

### 窗口状态

- `GetWindowLong()`/`SetWindowLong()`

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

## 流程

- application window
- client area
- control windows
- parent window
- owner window
- `HWND`
- Coordinates
- `CreateWindow` `CreateWindowEx`
- `MoveWindow`
- `GetCommandLine` `CommandLineToArgvW`
- `WinMain`

1. RegisterClass
2. CreateWindowEx
3. ShowWindow
4. GetMessage
5. DispatchMessage
6. WindowProc
7. WM_PAINT
8. WM_CLOSE
9. WM_DESTROY

DWM
render target
device
resource(device-dependent or device-independent)

## 图形界面绘制原理

- 应用程序调用 DWM API 管理自己的窗口，如位置、大小、最大化、最小化等
- 应用程序调用 Direct2D API 绘制窗口客户区

1. DWM(Desktop Windows Manager)负责管理应用程序窗口
2. 应用程序注册窗口处理函数并创建窗口
3. 操作系统为创建窗口的线程创建一个队列，用于保存该线程所创建窗口的所有窗口事件消息
4. 应用程序获取消息并交给窗口处理函数处理
5. 处理完成后返回结果码
6. DWM 获取结果码并将所有窗口的绘制层叠组合后输出到显卡
