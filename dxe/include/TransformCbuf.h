#pragma once

#include "ConstantBuffers.h"
#include "Drawable.h"

namespace dxe
{
	class TransformCbuf : public Bindable
	{
	public:
		TransformCbuf(Graphics& gfx, const Drawable& parent);
		void Bind(Graphics& gfx) noexcept override;
	private:
		static std::unique_ptr<VertexConstantBuffer<dx::XMMATRIX>> pVcbuf;
		const Drawable& parent;
	};

}
