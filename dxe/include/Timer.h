#pragma once

#include "DXE.h"

namespace dxe
{
	class Timer
	{
	public:
		Timer();
		float Mark();
		float Peek() const;
	private:
		std::chrono::steady_clock::time_point last;
	};
}
