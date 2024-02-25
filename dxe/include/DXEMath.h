#pragma once

#include "DXE.h"

namespace dxe
{
	constexpr f32 DXE_PI = 3.14159265;
	constexpr f64 DXE_PI_D = 3.1415926535897932;

	template <typename T>
	constexpr auto sq(const T& x)
	{
		return x * x;
	}

	template<typename T>
	T wrap_angle(T theta)
	{
		const T modded = fmod(theta, (T)2.0 * (T)DXE_PI_D);
		return (modded > (T)DXE_PI_D) ?
			(modded - (T)2.0 * (T)DXE_PI_D) :
			modded;
	}

	template<typename T>
	constexpr T interpolate(const T& src, const T& dst, f32 alpha)
	{
		return src + (dst - src) * alpha;
	}

	template<typename T>
	constexpr T to_rad(T deg)
	{
		return deg * DXE_PI / (T)180.0;
	}
}
