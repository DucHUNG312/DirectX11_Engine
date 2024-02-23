#pragma once

#include "DXE.h"
#include <dxgidebug.h>

namespace dxe
{
	class DxgiInfoManager
	{
	public:
		DxgiInfoManager();
		~DxgiInfoManager();
		DXE_NONCOPYABLE(DxgiInfoManager);
		void Set() noexcept;
		std::vector<std::string> GetMessages() const;
	private:
		unsigned long long next = 0u;
		struct IDXGIInfoQueue* pDxgiInfoQueue = nullptr;
	};

}

