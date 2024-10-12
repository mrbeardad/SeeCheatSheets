# DirectX

> https://learn.microsoft.com/en-us/windows/win32/directx

DirectX 包含了多套 API 用于增强图形和游戏的交互，其中最主要的就是 Direct3D

## COM

> https://learn.microsoft.com/en-us/windows/win32/prog-dx-with-com

DirectX API 是基于 COM 的

- 对象都直接或间接由工厂构造，构造时需要指定具体接口的 GUID `__uuidof(IDXGIFactory2)`
- 仅通过抽象接口与具体对象进行交互，可以尝试转换接口 `IUnknown::QueryInterface`
- 利用 COM 智能指针管理对象生命周期 `#include <wrl/client.h>` `Microsoft::WRL::ComPtr`
- 返回结果均为 `HRESULT`，可以使用宏 `SUCCEEDED` 和 `FAILED` 来检测结果

## Direct3D

### DXGI

> https://learn.microsoft.com/en-us/windows/win32/direct3ddxgi/d3d10-graphics-programming-guide-dxgi

从 Direct3D 10 开始，Direct3D 利用 DXGI 作为中间层来与显卡内核驱动交互

- `IDXGIFactory`
- `IDXGIAdapter`
  - `DXGI_ERROR_DEVICE_REMOVED` (must re-enumerate adapters again)
  - "Microsoft Basic Display Adapter" (`WARP` adapter)
- `IDXGIOutput`
- `IDXGISwapChain`
- `IDXGISurface`
