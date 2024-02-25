#pragma once
#include "Bindable.h"

namespace dxe
{
	class IndexBuffer : public Bindable
	{
	public:
		IndexBuffer(Graphics& gfx, const std::vector<u16>& indices);
		void Bind(Graphics& gfx) noexcept override;
		UINT GetCount() const noexcept;
	protected:
		UINT count;
		wrl::ComPtr<ID3D11Buffer> pIndexBuffer;
	};
}

