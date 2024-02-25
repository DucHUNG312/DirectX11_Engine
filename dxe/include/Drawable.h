#pragma once

#include "DXE.h"
#include "Graphics.h"

namespace dxe
{
	class Bindable;
	class IndexBuffer;

	class Drawable
	{
	public:
		Drawable() = default;
		Drawable(const Drawable&) = delete;
		virtual DirectX::XMMATRIX GetTransformXM() const noexcept = 0;
		void Draw(Graphics& gfx) const noexcept(!DXE_IS_DEBUG);
		virtual void Update(float dt) noexcept = 0;
		void AddBind(std::unique_ptr<Bindable> bind) noexcept(!DXE_IS_DEBUG);
		virtual ~Drawable() = default;
	private:
		const IndexBuffer* pIndexBuffer = nullptr;
		std::vector<std::unique_ptr<Bindable>> binds;
	};
}
