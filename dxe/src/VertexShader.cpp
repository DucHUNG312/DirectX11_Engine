#include "VertexShader.h"
#include "DXED3DCompiler.h"

namespace dxe
{
	VertexShader::VertexShader(Graphics& gfx, const std::wstring& path)
	{
		DXE_GFX_THROW_INFO(D3DReadFileToBlob(path.c_str(), &pBytecodeBlob));
		DXE_GFX_THROW_INFO(GetDevice(gfx)->CreateVertexShader(
			pBytecodeBlob->GetBufferPointer(),
			pBytecodeBlob->GetBufferSize(),
			nullptr,
			&pVertexShader
		));
	}

	void VertexShader::Bind(Graphics& gfx) noexcept
	{
		GetContext(gfx)->VSSetShader(pVertexShader.Get(), nullptr, 0u);
	}

	ID3DBlob* VertexShader::GetBytecode() const noexcept
	{
		return pBytecodeBlob.Get();
	}
}
