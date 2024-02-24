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
		void DrawTestTriangle();
		
	private:
#ifdef DXE_DEBUG_BUILD
		DxgiInfoManager infoManager;
#endif // DXE_DEBUG_BUILD
		wrl::ComPtr<ID3D11Device> pDevice = nullptr;
		wrl::ComPtr<IDXGISwapChain> pSwap = nullptr;
		wrl::ComPtr<ID3D11DeviceContext> pContext = nullptr;
		wrl::ComPtr<ID3D11RenderTargetView> pTarget = nullptr;
	};
}

