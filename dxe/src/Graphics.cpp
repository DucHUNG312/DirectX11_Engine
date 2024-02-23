#include "Graphics.h"

#pragma comment(lib, "d3d11.lib")

#ifdef DXE_DEBUG_BUILD
#define DXE_GFX_EXCEPT(hr) ::dxe::GfxHrException( __LINE__,__FILE__,(hr), ::dxe::Graphics::infoManager.GetMessages() )
#define DXE_GFX_THROW_INFO(hrcall) ::dxe::Graphics::infoManager.Set(); if( FAILED( hr = (hrcall) ) ) throw DXE_GFX_EXCEPT(hr)
#define DXE_GFX_DEVICE_REMOVED_EXCEPT(hr) ::dxe::DeviceRemovedException( __LINE__,__FILE__,(hr),::dxe::Graphics::infoManager.GetMessages() )
#else
#define DXE_GFX_EXCEPT(hr)
#define DXE_GFX_THROW_INFO(hrcall)
#define DXE_GFX_DEVICE_REMOVED_EXCEPT(hr)
#endif

namespace dxe
{
	Graphics::Graphics(HWND hWnd)
	{
		DXGI_SWAP_CHAIN_DESC sd = {};
		sd.BufferDesc.Width = 0;
		sd.BufferDesc.Height = 0;
		sd.BufferDesc.Format = DXGI_FORMAT_B8G8R8A8_UNORM;
		sd.BufferDesc.RefreshRate.Numerator = 0;
		sd.BufferDesc.RefreshRate.Denominator = 0;
		sd.BufferDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;
		sd.BufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
		sd.SampleDesc.Count = 1;
		sd.SampleDesc.Quality = 0;
		sd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
		sd.BufferCount = 1; // double buffering
		sd.OutputWindow = hWnd;
		sd.Windowed = TRUE;
		sd.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;
		sd.Flags = 0;

		UINT swapCreateFlags = 0u;
#ifdef DXE_DEBUG_BUILD
		swapCreateFlags |= D3D11_CREATE_DEVICE_DEBUG;
#endif

		// create device and front/back buffers, and swap chain and rendering context
		HRESULT hr;
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
		ID3D11Resource* pBackBuffer = nullptr;
		DXE_GFX_THROW_INFO(pSwap->GetBuffer(0, __uuidof(ID3D11Resource), reinterpret_cast<void**>(&pBackBuffer)));
		DXE_GFX_THROW_INFO(pDevice->CreateRenderTargetView(
			pBackBuffer,
			nullptr,
			&pTarget
		));
		pBackBuffer->Release();
	}

	Graphics::~Graphics()
	{
		if (pTarget != nullptr)
		{
			pTarget->Release();
		}
		if (pContext != nullptr)
		{
			pContext->Release();
		}
		if (pSwap != nullptr)
		{
			pSwap->Release();
		}
		if (pDevice != nullptr)
		{
			pDevice->Release();
		}
	}

	void Graphics::EndFrame()
	{
		HRESULT hr;
#ifdef DXE_DEBUG_BUILD
		infoManager.Set();
#endif // DXE_DEBUG_BUILD

		if (FAILED(hr == pSwap->Present(1u, 0u)))
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
		pContext->ClearRenderTargetView(pTarget, color);
	}
}
