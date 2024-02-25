#pragma once

#include "DXE.h"

namespace dxe
{
	DXE_FORCE_INLINE DXGI_SWAP_CHAIN_DESC& CreateSwapChainDescriptor(
		HWND hWnd,
		DXGI_FORMAT format,
		UINT width = 0,
		UINT height = 0,
		UINT sampleCount = 1,
		UINT sampleQuality = 0,
		UINT refNum = 0,
		UINT refDen = 0,
		UINT bufferCount = 1,
		DXGI_USAGE bufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT,
		DXGI_MODE_SCALING scalingMode = DXGI_MODE_SCALING_UNSPECIFIED,
		DXGI_MODE_SCANLINE_ORDER scanlineOrder = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED,
		BOOL windowed = TRUE,
		DXGI_SWAP_EFFECT swapEffect = DXGI_SWAP_EFFECT_DISCARD,
		UINT flags = 0
	)
	{
		DXGI_SWAP_CHAIN_DESC sd = {};
		sd.OutputWindow = hWnd;
		sd.BufferDesc.Format = format;
		sd.BufferDesc.Width = width;
		sd.BufferDesc.Height = height;
		sd.SampleDesc.Count = sampleCount;
		sd.SampleDesc.Quality = sampleQuality;
		sd.BufferDesc.RefreshRate.Numerator = refNum;
		sd.BufferDesc.RefreshRate.Denominator = refDen;
		sd.BufferCount = bufferCount;
		sd.BufferUsage = bufferUsage;
		sd.BufferDesc.Scaling = scalingMode;
		sd.BufferDesc.ScanlineOrdering = scanlineOrder;
		sd.Windowed = windowed;
		sd.SwapEffect = swapEffect;
		sd.Flags = flags;

		return sd;
	}

	DXE_FORCE_INLINE D3D11_DEPTH_STENCIL_DESC& CreateDepthStencilDescriptor(BOOL depthEnable = TRUE)
	{
		D3D11_DEPTH_STENCIL_DESC dsDesc = {};
		dsDesc.DepthEnable = depthEnable;
		dsDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
		dsDesc.DepthFunc = D3D11_COMPARISON_LESS;
		return dsDesc;
	}

	DXE_FORCE_INLINE D3D11_TEXTURE2D_DESC& CreateTextureDescriptor(
		UINT width,
		UINT height,
		DXGI_FORMAT format,
		D3D11_USAGE usage = D3D11_USAGE_DEFAULT,
		UINT bindFlags = D3D11_BIND_DEPTH_STENCIL,
		UINT mipLevels = 1u,
		UINT arraySize = 1u,
		UINT sampleCount = 1u,
		UINT sampleQuality = 0u
	)
	{
		D3D11_TEXTURE2D_DESC descDepth = {};
		descDepth.Width = width;
		descDepth.Height = height;
		descDepth.Format = format;
		descDepth.Usage = usage;
		descDepth.BindFlags = bindFlags;
		descDepth.MipLevels = mipLevels;
		descDepth.ArraySize = arraySize;
		descDepth.SampleDesc.Count = sampleCount;
		descDepth.SampleDesc.Quality = sampleQuality;
		return descDepth;
	}

	DXE_FORCE_INLINE D3D11_DEPTH_STENCIL_VIEW_DESC& CreateCreateDepthStencilViewDescriptor(
		DXGI_FORMAT format,
		D3D11_DSV_DIMENSION viewDimension,
		UINT mipSlice2D = 0u
		)
	{
		D3D11_DEPTH_STENCIL_VIEW_DESC descDSV = {};
		descDSV.Format = format;
		descDSV.ViewDimension = viewDimension;
		descDSV.Texture2D.MipSlice = mipSlice2D;
		return descDSV;
	}

	DXE_FORCE_INLINE D3D11_BUFFER_DESC& CreateBufferDescriptor(
		D3D11_BIND_FLAG bindFlags,
		UINT byteWidth, 
		UINT byteStride = 0u,
		D3D11_USAGE usage = D3D11_USAGE_DEFAULT,
		UINT cpuFlags = 0,
		UINT miscFlags = 0u
		)
	{
		D3D11_BUFFER_DESC cbd = {};
		cbd.BindFlags = bindFlags;
		cbd.ByteWidth = byteWidth;
		cbd.StructureByteStride = byteStride;
		cbd.Usage = usage;
		cbd.CPUAccessFlags = cpuFlags;
		cbd.MiscFlags = miscFlags;
		return cbd;
	}
}
