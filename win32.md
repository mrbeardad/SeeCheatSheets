# Win32

BYTE DWORD BOOL P*

PSTR PCSTR PWSTR PCWSTR

TCHAR TEXT SetWindowText SetWindowTextA SetWindowTextW 

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
6. DWM获取结果码并将所有窗口的绘制层叠组合后输出到显卡
