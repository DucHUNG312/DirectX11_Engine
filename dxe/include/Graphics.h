#pragma once
#include "DXE.h"
#include "DXEWin.h"
#include <d3d11.h>

namespace dxe
{
	class Graphics
	{
	public:
		Graphics(HWND hWnd);
		~Graphics();
		DXE_NONCOPYABLE(Graphics);

		void EndFrame();
		void ClearBuffer(f32 red, f32 green, f32 blue) noexcept;
	private:
		ID3D11Device* pDevice = nullptr;
		IDXGISwapChain* pSwap = nullptr;
		ID3D11DeviceContext* pContext = nullptr;
		ID3D11RenderTargetView* pTarget = nullptr;
	};
}

