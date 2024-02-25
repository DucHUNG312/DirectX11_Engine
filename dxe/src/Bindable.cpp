#include "Bindable.h"

namespace dxe
{
	ID3D11DeviceContext* Bindable::GetContext(Graphics& gfx) noexcept
	{
		return gfx.pContext.Get();
	}

	ID3D11Device* Bindable::GetDevice(Graphics& gfx) noexcept
	{
		return gfx.pDevice.Get();
	}

	DxgiInfoManager& Bindable::GetInfoManager(Graphics& gfx) noexcept(!DXE_IS_DEBUG)
	{
#ifdef DXE_DEBUG_BUILD
		return gfx.infoManager;
#else
		DXE_CHECK_MSG(false, "Error: tried to access gfx.infoManager in Release config");
#endif
	}
}
