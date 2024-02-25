#include "IndexBuffer.h"

namespace dxe
{
	IndexBuffer::IndexBuffer(Graphics& gfx, const std::vector<u16>& indices)
		: count((UINT)indices.size())
	{
		/// TODO
		D3D11_BUFFER_DESC ibd = CreateBufferDescriptor(D3D11_BIND_INDEX_BUFFER, UINT(count * sizeof(u16)), sizeof(u16));
		D3D11_SUBRESOURCE_DATA isd = {};
		isd.pSysMem = indices.data();
		DXE_GFX_THROW_INFO(GetDevice(gfx)->CreateBuffer(&ibd, &isd, &pIndexBuffer));
	}

	void IndexBuffer::Bind(Graphics& gfx) noexcept
	{
		GetContext(gfx)->IASetIndexBuffer(pIndexBuffer.Get(), DXGI_FORMAT_R16_UINT, 0u);
	}

	UINT IndexBuffer::GetCount() const noexcept
	{
		return count;
	}
}
