#include "Drawable.h"
#include "IndexBuffer.h"

namespace dxe
{
	void Drawable::Draw(Graphics& gfx) const noexcept(!DXE_IS_DEBUG)
	{
		for (auto& b : binds)
		{
			b->Bind(gfx);
		}
		gfx.DrawIndexed(pIndexBuffer->GetCount());
	}

	void Drawable::AddBind(std::unique_ptr<Bindable> bind) noexcept(!DXE_IS_DEBUG)
	{
		if (typeid(*bind) == typeid(IndexBuffer))
		{
			DXE_CHECK_MSG(pIndexBuffer == nullptr, "Attempting to add index buffer a second time");
			pIndexBuffer = dynamic_cast<IndexBuffer*>(bind.get());
			if (!pIndexBuffer) 
			{
				DXE_CHECK_MSG(false, "Failed to cast Bindable pointer to IndexBuffer pointer");
				DXE_RUNTIME_THROW;
			}
		}
		binds.push_back(std::move(bind));
	}
}
