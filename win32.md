# Win32

## 常见类型

```cpp
// 匈牙利命名法

BYTE DWORD BOOL P*

PSTR PCSTR PWSTR PCWSTR

TCHAR TEXT

SetWindowText SetWindowTextA SetWindowTextW
```

## 窗口

- 窗口类型

  - Desktop Window
  - Top-Level Window
    - Overlapped Window
      - `WS_OVERLAPPED`: title bar, border
      - `WS_OVERLAPPEDWINDOW`: title bar, window menu, window buttions, sizing border
    - Pop-up Window
      - `WS_POPUP`: only client area
      - `WS_POPUP | WS_CAPTION`: title bar
      - `WS_POPUPWINDOW`: title bar, border
      - `WS_POPUPWINDOW | WS_CAPTION`: title bar, window menu, border
  - Child Window
    - `WS_CHILD`: only client area
    - parent client area related coordinate
    - show, hide, move, destroy with parent
    - clip: outside parent client area, `WS_CLIPCHILDREN`, `WS_CLIPSIBLINGS`
    - `SetParent`: 将子窗口删除并移动到新父窗口中，父窗口置空则成为 top-level window
  - Owned Window
    - always above owern
    - destroy with owern
    - hide when owern minimize and show when owern resotre
  - Layerd Windows:
    - 扩展样式设为`WS_EX_LAYERED`
    - 复杂形状、动画形状、alpha 混合
    - 0 alpha 像素透过鼠标事件
    - `WS_EX_TRANSPARENT`窗口透过鼠标事件
  - Message-Only Windows:
    - 父窗口设为`HWND_MESSAGE`

- Owned Window

  - always in front of owner
  - hidden and destroyed with owner

- 窗口区域

  - Application Window (Main Window)
    - Nonclient Areas
      - title bar
      - window menu
      - minimize, maximize and restore buttons
      - menu bar
      - (sizing) border
      - scroll bars
    - Client Area

```cpp
WS_POPUP
WS_POPUPWINDOW
WS_CAPTION
WS_CHILD
```

- 窗口样式
  - `WS_OVERLAPPED`
    - title bar
    - border
  - `WS_OVERLAPPEDWINDOW`
    - title bar
    - minimize and maximize buttons
    - window menu
    - sizing border
  - Control
  - Dialog Boxes
  - Message Boxes

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
