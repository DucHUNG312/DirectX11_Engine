#include "PixelShader.h"
#include "DXED3DCompiler.h"

namespace dxe
{
	PixelShader::PixelShader(Graphics& gfx, const std::wstring& path)
	{
		wrl::ComPtr<ID3DBlob> pBlob;
		DXE_GFX_THROW_INFO(D3DReadFileToBlob(path.c_str(), &pBlob));
		DXE_GFX_THROW_INFO(GetDevice(gfx)->CreatePixelShader(pBlob->GetBufferPointer(), pBlob->GetBufferSize(), nullptr, &pPixelShader));
	}

	void PixelShader::Bind(Graphics& gfx) noexcept
	{
		GetContext(gfx)->PSSetShader(pPixelShader.Get(), nullptr, 0u);
	}
}
