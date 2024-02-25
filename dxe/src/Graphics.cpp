#include "Graphics.h"

#pragma comment(lib, "d3d11.lib")

namespace dxe
{
	DxgiInfoManager Graphics::infoManager = DxgiInfoManager{};

	Graphics::Graphics(HWND& hWnd, i32 width, i32 height)
		: hWnd(hWnd), viewportWidth(width), viewportHeight(height)
	{
		CreateSwapchainAndDevice();

		CreateDepthStensil();

		ConfigureViewPort();
	}

	void Graphics::CreateSwapchainAndDevice()
	{
		DXGI_SWAP_CHAIN_DESC sd = CreateSwapChainDescriptor(hWnd, DXGI_FORMAT_B8G8R8A8_UNORM);

		UINT swapCreateFlags = 0u;
#ifdef DXE_DEBUG_BUILD
		swapCreateFlags |= D3D11_CREATE_DEVICE_DEBUG;
#endif

		// create device and front/back buffers, and swap chain and rendering context
		DXE_GFX_THROW_INFO(D3D11CreateDeviceAndSwapChain(
			nullptr,
			D3D_DRIVER_TYPE_HARDWARE,
			nullptr,
			swapCreateFlags,
			nullptr,
			0,
			D3D11_SDK_VERSION,
			&sd,
			&pSwap,
			&pDevice,
			nullptr,
			&pContext
		));

		// gain access to texture subresource in swap chain (back buffer)
		wrl::ComPtr<ID3D11Resource> pBackBuffer;
		DXE_GFX_THROW_INFO(pSwap->GetBuffer(0, __uuidof(ID3D11Resource), &pBackBuffer));
		DXE_GFX_THROW_INFO(pDevice->CreateRenderTargetView(
			pBackBuffer.Get(),
			nullptr,
			&pTarget
		));
	}

	void Graphics::CreateDepthStensil()
	{
		// create depth stensil state
		D3D11_DEPTH_STENCIL_DESC dsDesc = CreateDepthStencilDescriptor();
		wrl::ComPtr<ID3D11DepthStencilState> pDSState;
		DXE_GFX_THROW_INFO(pDevice->CreateDepthStencilState(&dsDesc, &pDSState));

		// bind depth state
		pContext->OMSetDepthStencilState(pDSState.Get(), 1u);

		// create depth stensil texture
		wrl::ComPtr<ID3D11Texture2D> pDepthStencil;
		D3D11_TEXTURE2D_DESC descDepth = CreateTextureDescriptor(viewportWidth, viewportHeight, DXGI_FORMAT_D32_FLOAT);
		DXE_GFX_THROW_INFO(pDevice->CreateTexture2D(&descDepth, nullptr, &pDepthStencil));

		// create view of depth stensil texture
		D3D11_DEPTH_STENCIL_VIEW_DESC descDSV = CreateCreateDepthStencilViewDescriptor(DXGI_FORMAT_D32_FLOAT, D3D11_DSV_DIMENSION_TEXTURE2D);
		DXE_GFX_THROW_INFO(pDevice->CreateDepthStencilView(
			pDepthStencil.Get(), &descDSV, &pDSV
		));

		// bind depth stensil view to OM
		pContext->OMSetRenderTargets(1u, pTarget.GetAddressOf(), pDSV.Get());
	}

	void Graphics::ConfigureViewPort()
	{
		// configure viewport
		D3D11_VIEWPORT vp;
		vp.Width = viewportWidth;
		vp.Height = viewportHeight;
		vp.MinDepth = 0.0f;
		vp.MaxDepth = 1.0f;
		vp.TopLeftX = 0.0f;
		vp.TopLeftY = 0.0f;
		pContext->RSSetViewports(1u, &vp);
	}

	void Graphics::EndFrame()
	{
		HRESULT hr;
#ifdef DXE_DEBUG_BUILD
		infoManager.Set();
#endif // DXE_DEBUG_BUILD

		if (FAILED(hr = pSwap->Present(1u, 0u)))
		{
			if (hr == DXGI_ERROR_DEVICE_REMOVED)
			{
				throw DXE_GFX_DEVICE_REMOVED_EXCEPT(pDevice->GetDeviceRemovedReason());
			}
			else
			{
				throw DXE_GFX_EXCEPT(hr);
			}
		}
	}

	void Graphics::ClearBuffer(f32 red, f32 green, f32 blue) noexcept
	{
		const f32 color[] = { red,green,blue,1.0f };
		pContext->ClearRenderTargetView(pTarget.Get(), color);
		pContext->ClearDepthStencilView(pDSV.Get(), D3D11_CLEAR_DEPTH, 1.0f, 0u);
	}

	void Graphics::DrawIndexed(UINT count) noexcept(!DXE_IS_DEBUG)
	{
		DXE_GFX_THROW_INFO_ONLY(pContext->DrawIndexed(count, 0u, 0u));
	}

	void Graphics::SetProjection(dx::FXMMATRIX proj) noexcept
	{
		projection = proj;
	}

	dx::XMMATRIX Graphics::GetProjection() const noexcept
	{
		return projection;
	}

	DxgiInfoManager& Graphics::GetInfoManager() noexcept
	{
		return infoManager;
	}
	
}
