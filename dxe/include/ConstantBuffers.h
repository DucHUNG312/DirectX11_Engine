#pragma once

#include "DXE.h"
#include "Bindable.h"
#include "DescriptorManage.h"

namespace dxe
{
	template<typename C>
	class ConstantBuffer : public Bindable
	{
	public:
		void Update(Graphics& gfx, const C& consts)
		{
			D3D11_MAPPED_SUBRESOURCE msr;
			DXE_GFX_THROW_INFO(GetContext(gfx)->Map(
				pConstantBuffer.Get(), 0u,
				D3D11_MAP_WRITE_DISCARD, 0u,
				&msr
			));
			memcpy(msr.pData, &consts, sizeof(consts));
			GetContext(gfx)->Unmap(pConstantBuffer.Get(), 0u);
		}

		ConstantBuffer(Graphics& gfx, const C& consts)
		{
			D3D11_BUFFER_DESC cbd = CreateBufferDescriptor(D3D11_BIND_CONSTANT_BUFFER, sizeof(consts), 0, D3D11_USAGE_DYNAMIC, D3D10_CPU_ACCESS_WRITE);
			D3D11_SUBRESOURCE_DATA csd = {};
			csd.pSysMem = &consts;
			DXE_GFX_THROW_INFO(GetDevice(gfx)->CreateBuffer(&cbd, &csd, &pConstantBuffer));
		}

		ConstantBuffer(Graphics& gfx)
		{
			D3D11_BUFFER_DESC cbd = CreateBufferDescriptor(D3D11_BIND_CONSTANT_BUFFER, sizeof(C), 0, D3D11_USAGE_DYNAMIC, D3D10_CPU_ACCESS_WRITE);
			DXE_GFX_THROW_INFO(GetDevice(gfx)->CreateBuffer(&cbd, nullptr, &pConstantBuffer));
		}
	protected:
		wrl::ComPtr<ID3D11Buffer> pConstantBuffer;
	};

	template<typename C>
	class VertexConstantBuffer : public ConstantBuffer<C>
	{
		using ConstantBuffer<C>::pConstantBuffer;
		using Bindable::GetContext;
	public:
		using ConstantBuffer<C>::ConstantBuffer;
		void Bind(Graphics& gfx) noexcept override
		{
			GetContext(gfx)->VSSetConstantBuffers(0u, 1u, pConstantBuffer.GetAddressOf());
		}
	};

	template<typename C>
	class PixelConstantBuffer : public ConstantBuffer<C>
	{
		using ConstantBuffer<C>::pConstantBuffer;
		using Bindable::GetContext;
	public:
		using ConstantBuffer<C>::ConstantBuffer;
		void Bind(Graphics& gfx) noexcept override
		{
			GetContext(gfx)->PSSetConstantBuffers(0u, 1u, pConstantBuffer.GetAddressOf());
		}
	};
}
