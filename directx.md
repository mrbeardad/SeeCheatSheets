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

以下情况可能会触发错误 `DXGI_ERROR_DEVICE_REMOVED` 或 `DXGI_ERROR_DEVICE_RESET`

- 显卡驱动升级
- 系统切换使用显卡
- 显卡未响应或被重置
- 显卡被插入或移除

可以在 `Present` 和 `WM_SIZE > ResizeBuffers` 中检测并处理，届时需要销毁所有 DXGI 资源并重新创建

### SwapChian

> https://learn.microsoft.com/en-us/windows/win32/api/dxgi1_2/nf-dxgi1_2-idxgifactory2-createswapchainforhwnd  
> https://jackmin.home.blog/2018/12/14/swapchains-present-and-present-latency/  
> https://www.intel.com/content/www/us/en/developer/articles/code-sample/sample-application-for-direct3d-12-flip-model-swap-chains.html

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

SwapChain 有若干个缓冲区，通常会将其中一个作为渲染目标，渲染完成后会将缓冲区内容提交给 DWM 合成并显示到屏幕

- `IDXGIFactory::MakeWindowAssociation`：监听窗口消息，`Alt+Enter` 切换全屏模式，`Print-Screen` 截屏
- `IDXGISwapChain::SetFullscreenState`：切换全屏模式，通常良好的步骤如下
  1. `IDXGISwapChain::GetContainingOutput` 选择一个合适的屏幕
  2. `IDXGIOutput::FindClosestMatchingMode` 选择一个合适的屏幕分辨率
  3. `IDXGISwapChain::ResizeTarget` 调整窗口大小/屏幕分辨率，同时触发 `WM_SIZE` 消息
  4. 释放所有 SwapChain 相关的 Buffer
  5. `IDXGISwapChain::ResizeBuffers` 调整 Buffer 大小
  6. 重新获取 SwapChain 相关的 Buffer
  7. `IDXGISwapChain::SetFullscreenState` 切换全屏模式

> - `DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH` 该创建标志使得全屏模式下调用 `IDXGISwapChain::ResizeTarget` 会自动设置屏幕分辨率
> - 有窗口遮挡时自动退出全屏
> - 注意退出全屏模式后再释放 SwapChain

Flip Mode vs Blt Mode

`IDXGISwapChain1::Present1`

完整渲染工作流程：

1. CPU 为 GPU 渲染管线设置状态，这些命令都缓存到队列并最终一起提交给 GPU
2. GPU 等待渲染目标可用
3. GPU 渲染管线开始工作
4. GPU 渲染完成并将结果写入 SwapChain 缓冲区
5. GPU 提交 Present 队列 ...
