#pragma once
#include "DXE.h"
#include <d3d11.h>
#include "DxgiInfoManager.h"

namespace dxe
{
	class Graphics
	{
	public:
		Graphics(HWND hWnd);
		~Graphics() = default;
		DXE_NONCOPYABLE(Graphics);

		void EndFrame();
		void ClearBuffer(f32 red, f32 green, f32 blue) noexcept;
		void DrawTestTriangle(f32 angle, f32 x, f32 y);
		
	private:
#ifdef DXE_DEBUG_BUILD
		DxgiInfoManager infoManager;
#endif // DXE_DEBUG_BUILD
		wrl::ComPtr<ID3D11Device> pDevice;
		wrl::ComPtr<IDXGISwapChain> pSwap;
		wrl::ComPtr<ID3D11DeviceContext> pContext;
		wrl::ComPtr<ID3D11RenderTargetView> pTarget;
		wrl::ComPtr<ID3D11DepthStencilView> pDSV;
	};
}

