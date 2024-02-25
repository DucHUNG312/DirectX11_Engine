#include "TransformCbuf.h"

namespace dxe
{
	TransformCbuf::TransformCbuf(Graphics& gfx, const Drawable& parent)
		:
		vcbuf(gfx),
		parent(parent)
	{}

	void TransformCbuf::Bind(Graphics& gfx) noexcept
	{
		vcbuf.Update(gfx,
			dx::XMMatrixTranspose(
				parent.GetTransformXM() * gfx.GetProjection()
			)
		);
		vcbuf.Bind(gfx);
	}
}
