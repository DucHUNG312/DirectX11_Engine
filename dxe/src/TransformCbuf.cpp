#include "TransformCbuf.h"

namespace dxe
{
	std::unique_ptr<VertexConstantBuffer<dx::XMMATRIX>> TransformCbuf::pVcbuf;

	TransformCbuf::TransformCbuf(Graphics& gfx, const Drawable& parent)
		: parent(parent)
	{
		if (!pVcbuf)
		{
			pVcbuf = std::make_unique<VertexConstantBuffer<dx::XMMATRIX>>(gfx);
		}
	}

	void TransformCbuf::Bind(Graphics& gfx) noexcept
	{
		pVcbuf->Update(gfx,
			dx::XMMatrixTranspose(
				parent.GetTransformXM() * gfx.GetProjection()
			)
		);
		pVcbuf->Bind(gfx);
	}
}
