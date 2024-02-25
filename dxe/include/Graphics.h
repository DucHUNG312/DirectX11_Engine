#pragma once
#include "DXE.h"
#include "DescriptorManage.h"
#include "DxgiInfoManager.h"

namespace dxe
{
	class Graphics
	{
		friend class Bindable;
	public:
		Graphics(HWND& hWnd, i32 width, i32 height);
		~Graphics() = default;
		DXE_NONCOPYABLE(Graphics);

		void EndFrame();
		void ClearBuffer(f32 red, f32 green, f32 blue) noexcept;
		void DrawIndexed(UINT count) noexcept(!DXE_IS_DEBUG);
		void SetProjection(dx::FXMMATRIX proj) noexcept;
		dx::XMMATRIX GetProjection() const noexcept;
		static DxgiInfoManager& GetInfoManager() noexcept;
	private:
		void CreateSwapchainAndDevice();
		void CreateDepthStensil();
		void ConfigureViewPort();
	private:
		HWND& hWnd;
		i32 viewportWidth, viewportHeight;
		static DxgiInfoManager infoManager;
		dx::XMMATRIX projection;
#ifdef DXE_DEBUG_BUILD
#endif // DXE_DEBUG_BUILD
		wrl::ComPtr<ID3D11Device> pDevice;
		wrl::ComPtr<IDXGISwapChain> pSwap;
		wrl::ComPtr<ID3D11DeviceContext> pContext;
		wrl::ComPtr<ID3D11RenderTargetView> pTarget;
		wrl::ComPtr<ID3D11DepthStencilView> pDSV;
	};
}

