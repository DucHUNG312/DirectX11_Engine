#pragma once

#include "DXE.h"
#include "Graphics.h"

namespace dxe
{
	class Bindable;
	class IndexBuffer;

	class Drawable
	{
		template<class T>
		friend class DrawableBase;
	public:
		Drawable() = default;
		Drawable(const Drawable&) = delete;
		virtual DirectX::XMMATRIX GetTransformXM() const noexcept = 0;
		void Draw(Graphics& gfx) const noexcept(!DXE_IS_DEBUG);
		virtual void Update(float dt) noexcept = 0;
		void AddBind(std::unique_ptr<Bindable> bind) noexcept(!DXE_IS_DEBUG);
		virtual ~Drawable() = default;
	protected:
		virtual const std::vector<std::unique_ptr<Bindable>>& GetStaticBinds() const noexcept = 0;
		const IndexBuffer* pIndexBuffer = nullptr;
		std::vector<std::unique_ptr<Bindable>> binds;
	};
}
