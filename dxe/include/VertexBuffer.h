#pragma once

#include "Bindable.h"

namespace dxe
{
	class VertexBuffer : public Bindable
	{
	public:
		template<class V>
		VertexBuffer(Graphics& gfx, const std::vector<V>& vertices)
			:
			stride(sizeof(V))
		{
			/// TODO: Check CPU Access
			D3D11_BUFFER_DESC bd = CreateBufferDescriptor(D3D11_BIND_VERTEX_BUFFER, UINT(sizeof(V) * vertices.size()), sizeof(V));
			D3D11_SUBRESOURCE_DATA sd = {};
			sd.pSysMem = vertices.data();
			DXE_GFX_THROW_INFO(GetDevice(gfx)->CreateBuffer(&bd, &sd, &pVertexBuffer));
		}
		void Bind(Graphics& gfx) noexcept override;
	protected:
		UINT stride;
		wrl::ComPtr<ID3D11Buffer> pVertexBuffer;
	};
}
