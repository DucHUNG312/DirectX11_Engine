#include "Graphics.h"
#include <d3dcompiler.h>

#pragma comment(lib, "d3d11.lib")
#pragma comment(lib, "D3DCompiler.lib")

#ifdef DXE_DEBUG_BUILD
#define DXE_GFX_EXCEPT(hr) ::dxe::GfxHrException( __LINE__,__FILE__,(hr), ::dxe::Graphics::infoManager.GetMessages() )
#define DXE_GFX_THROW_INFO(hrcall)			{																											\
												HRESULT hr;																								\
												::dxe::Graphics::infoManager.Set();																		\
												if( FAILED( hr = (hrcall) ) )																			\
												{																										\
													throw DXE_GFX_EXCEPT(hr);																			\
												}																										\
											}
#define DXE_GFX_DEVICE_REMOVED_EXCEPT(hr)   ::dxe::DeviceRemovedException( __LINE__,__FILE__,(hr),::dxe::Graphics::infoManager.GetMessages() )
#define DXE_GFX_THROW_INFO_ONLY(call)       ::dxe::Graphics::infoManager.Set();																			\
											(call);																										\
											{																											\
												auto v = ::dxe::Graphics::infoManager.GetMessages();													\
												if(!v.empty())																							\
												{																										\
													throw ::dxe::InfoException( __LINE__,__FILE__,v);													\
												}																										\
											}
#else
#define DXE_GFX_EXCEPT(hr)
#define DXE_GFX_THROW_INFO(hrcall)
#define DXE_GFX_DEVICE_REMOVED_EXCEPT(hr)
#define DXE_GFX_THROW_INFO_ONLY(call)
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
	}

	void Graphics::DrawTestTriangle()
	{
		struct Vertex
		{
			f32 x;
			f32 y;
		};

		// create vertex buffer (1 2d triangle at center of screen)
		const Vertex vertices[] =
		{
			{ 0.0f,0.5f },
			{ 0.5f,-0.5f },
			{ -0.5f,-0.5f },
		};

		wrl::ComPtr<ID3D11Buffer> pVertexBuffer;
		D3D11_BUFFER_DESC bd = {};
		bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
		bd.Usage = D3D11_USAGE_DEFAULT;
		bd.CPUAccessFlags = 0u;
		bd.MiscFlags = 0u;
		bd.ByteWidth = sizeof(vertices);
		bd.StructureByteStride = sizeof(Vertex);

		D3D11_SUBRESOURCE_DATA sd = {};
		sd.pSysMem = vertices;

		DXE_GFX_THROW_INFO(pDevice->CreateBuffer(&bd, &sd, &pVertexBuffer));

		// Bind vertex buffer to pipeline
		const UINT stride = sizeof(Vertex);
		const UINT offset = 0u;
		pContext->IASetVertexBuffers(0u, 1u, pVertexBuffer.GetAddressOf(), &stride, &offset);

		// create pixel shader
		wrl::ComPtr<ID3D11PixelShader> pPixelShader;
		wrl::ComPtr<ID3DBlob> pBlob;
		DXE_GFX_THROW_INFO(D3DReadFileToBlob(L"shadersout\\PixelShader.p.cso", &pBlob));
		DXE_GFX_THROW_INFO(pDevice->CreatePixelShader(pBlob->GetBufferPointer(), pBlob->GetBufferSize(), nullptr, &pPixelShader));

		// bind pixel shader
		pContext->PSSetShader(pPixelShader.Get(), nullptr, 0u);

		// create vertex shader
		wrl::ComPtr<ID3D11VertexShader> pVertexShader;
		DXE_GFX_THROW_INFO(D3DReadFileToBlob(L"shadersout\\VertexShader.v.cso", &pBlob));
		DXE_GFX_THROW_INFO(pDevice->CreateVertexShader(pBlob->GetBufferPointer(), pBlob->GetBufferSize(), nullptr, &pVertexShader));

		// bind vertex shader
		pContext->VSSetShader(pVertexShader.Get(), nullptr, 0u);

		// input (vertex) layout (2d position only)
		wrl::ComPtr<ID3D11InputLayout> pInputLayout;
		const D3D11_INPUT_ELEMENT_DESC ied[] =
		{
			{ "Position",0,DXGI_FORMAT_R32G32_FLOAT,0,0,D3D11_INPUT_PER_VERTEX_DATA,0 },
		};
		DXE_GFX_THROW_INFO(pDevice->CreateInputLayout(
			ied, (UINT)std::size(ied),
			pBlob->GetBufferPointer(),
			pBlob->GetBufferSize(),
			&pInputLayout
		));

		// bind vertex layout
		pContext->IASetInputLayout(pInputLayout.Get());
		
		// bind render target
		pContext->OMSetRenderTargets(1u, pTarget.GetAddressOf(), nullptr);

		// Set primitive topology to triangle list (groups of 3 vertices)
		pContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

		// configure viewport
		D3D11_VIEWPORT vp;
		vp.Width = 800;
		vp.Height = 600;
		vp.MinDepth = 0;
		vp.MaxDepth = 1;
		vp.TopLeftX = 0;
		vp.TopLeftY = 0;
		pContext->RSSetViewports(1u, &vp);


		DXE_GFX_THROW_INFO_ONLY(pContext->Draw((UINT)std::size(vertices), 0u));
	}
}
