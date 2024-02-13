#include "Timer.h"

namespace dxe
{
	Timer::Timer()
	{
		last = std::chrono::steady_clock::now();
	}

	f32 Timer::Mark()
	{
		const auto old = last;
		last = std::chrono::steady_clock::now();
		const std::chrono::duration<f32> frameTime = last - old;
		return frameTime.count();
	}

	f32 Timer::Peek() const
	{
		return std::chrono::duration<f32>(std::chrono::steady_clock::now() - last).count();
	}
}
