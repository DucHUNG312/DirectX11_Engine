#pragma once

#include "DXE.h"
#include <wrl.h>
#include <dxgidebug.h>

namespace wrl = Microsoft::WRL;

namespace dxe
{
	class DxgiInfoManager
	{
	public:
		DxgiInfoManager();
		~DxgiInfoManager() = default;
		DXE_NONCOPYABLE(DxgiInfoManager);
		void Set() noexcept;
		std::vector<std::string> GetMessages() const;
	private:
		unsigned long long next = 0u;
		wrl::ComPtr<IDXGIInfoQueue> pDxgiInfoQueue;
	};

}

