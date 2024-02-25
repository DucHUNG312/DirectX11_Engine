#pragma once

#include "Bindable.h"

namespace dxe
{
	class VertexShader : public Bindable
	{
	public:
		VertexShader(Graphics& gfx, const std::wstring& path);
		void Bind(Graphics& gfx) noexcept override;
		ID3DBlob* GetBytecode() const noexcept;
	protected:
		wrl::ComPtr<ID3DBlob> pBytecodeBlob;
		wrl::ComPtr<ID3D11VertexShader> pVertexShader;
	};
}
