# DirectX

> https://learn.microsoft.com/en-us/windows/win32/directx

DirectX 包含了多套 API 用于增强图形和游戏的交互，其中最主要的就是 Direct3D

## COM

> https://learn.microsoft.com/en-us/windows/win32/prog-dx-with-com

DirectX API 是基于 COM 的

- 对象都直接或间接由工厂构造，构造时需要指定抽象接口的 GUID `__uuidof(IDXGIFactory2)`
- 仅通过抽象接口与具体对象进行交互，可以尝试转换接口来扩展可用功能 `IUnknown::QueryInterface`
- 利用 COM 智能指针管理对象生命周期 `#include <wrl/client.h>` `Microsoft::WRL::ComPtr`
- 返回结果均为 `HRESULT`，可以使用宏 `SUCCEEDED` 和 `FAILED` 来检测结果

## Direct3D

### DXGI

> https://learn.microsoft.com/en-us/windows/win32/direct3ddxgi/d3d10-graphics-programming-guide-dxgi

![dxgi](images/dxgi.png)

从 Direct3D 10 开始，Direct3D 利用 DXGI 作为中间层来与显卡内核驱动交互

- `IDXGIFactory`
  - `IDXGIAdapter`：代表显卡
    - `IDXGIOutput`：代表屏幕
    - `IDXGIDevice`：与显卡交互的接口
      - `IDXGISwapChain`：与 DWM 交互的接口
      - `IDXGISurface`：代表显存资源

> - 不要混用 DXGI 1.0 (IDXGIFactory) 和 DXGI 1.1 (IDXGIFactory1)
> - 不要在 DllMain 中调用 DXGI
> - 当渲染线程和窗口线程分开时，渲染线程可能会同步等待窗口线程 (SendMessage)，注意避免死锁

#### IDXGISwapChian

为使用新的 SwapChain 构造函数（如 `CreateSwapChainForHwnd`），通常需要如下步骤

- D3D11

  1. `ID3D11Device::QueryInterface`
  2. `IDXGIDevice::GetAdapter`
  3. `IDXGIAdapter::GetParent`
  4. `IDXGIFactory2::CreateSwapChainForHwnd`

- D3D12
  1. `ID3D12Device::GetAdapterLuid`
  2. `IDXGIFactory4::EnumAdapterByLuid`
  3. `IDXGIAdapter::GetParent`
  4. `IDXGIFactory2::CreateSwapChainForHwnd`

SwapChain 通常作为渲染目标，负责将渲染结果发送给 DWM 进行合成，最终显示到屏幕上

- `IDXGIFactory::MakeWindowAssociation`：监听窗口消息，`Alt+Enter` 切换全屏模式，`Print-Screen` 截屏
- `IDXGISwapChain::SetFullscreenState`：切换全屏模式，通常良好的步骤如下
  1. `IDXGISwapChain::GetContainingOutput` 选择一个合适的屏幕
  2. `IDXGIOutput::FindClosestMatchingMode` 选择一个合适的屏幕分辨率
  3. `IDXGISwapChain::ResizeTarget` 调整窗口大小/屏幕分辨率
  4. 发送 `WM_SIZE` 消息
  5. 释放所有 SwapChain 相关的 Buffer
  6. `IDXGISwapChain::ResizeBuffers` 调整 Buffer 大小
  7. 重新获取 SwapChain 相关的 Buffer
  8. `IDXGISwapChain::SetFullscreenState` 切换全屏模式
- `DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH` 该创建标志允许全屏模式下调用 `IDXGISwapChain::ResizeTarget` 自动设置屏幕分辨率
- 有窗口遮挡时自动退出全屏
- 注意推出全屏模式后再释放 SwapChain
