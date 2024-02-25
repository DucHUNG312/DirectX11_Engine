#pragma once
#include "Drawable.h"
#include "IndexBuffer.h"

namespace dxe
{
	template<class T>
	class DrawableBase : public Drawable
	{
	protected:
		static bool IsStaticInitialized() noexcept
		{
			return !staticBinds.empty();
		}
		void AddStaticBind(std::unique_ptr<Bindable> bind) noexcept(!DXE_IS_DEBUG)
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
			staticBinds.push_back(std::move(bind));
		}

		void SetIndexFromStatic() noexcept(!DXE_IS_DEBUG)
		{
			DXE_CHECK_MSG(pIndexBuffer == nullptr, "Attempting to add index buffer a second time");
			for (const auto& b : staticBinds)
			{
				if (const auto p = dynamic_cast<IndexBuffer*>(b.get()))
				{
					pIndexBuffer = p;
					return;
				}
			}
			DXE_CHECK_MSG(pIndexBuffer != nullptr, "Failed to find index buffer in static binds");
		}
	private:
		const std::vector<std::unique_ptr<Bindable>>& GetStaticBinds() const noexcept override
		{
			return staticBinds;
		}
	private:
		static std::vector<std::unique_ptr<Bindable>> staticBinds;
	};

	template<class T>
	std::vector<std::unique_ptr<Bindable>> DrawableBase<T>::staticBinds;
}

