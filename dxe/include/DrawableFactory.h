#pragma once

#include "DXE.h"
#include "DXEMath.h"
#include "Graphics.h"
#include "Drawable.h"
#include "Box.h"
#include "Melon.h"
#include "Pyramid.h"

namespace dxe
{
	class DrawableFactory
	{
	public:
		DrawableFactory(Graphics& gfx)
			:
			gfx(gfx)
		{}
		std::unique_ptr<Drawable> operator()()
		{
			switch (typedist(rng))
			{
			case 0:
				return std::make_unique<Pyramid>(
					gfx, rng, adist, ddist,
					odist, rdist
				);
			case 1:
				return std::make_unique<Box>(
					gfx, rng, adist, ddist,
					odist, rdist, bdist
				);
			case 2:
				return std::make_unique<Melon>(
					gfx, rng, adist, ddist,
					odist, rdist, longdist, latdist
				);
			default:
				DXE_CHECK(false && "bad drawable type in factory");
				return {};
			}
		}
	private:
		Graphics& gfx;
		std::mt19937 rng{ std::random_device{}() };
		std::uniform_real_distribution<f32> adist{ 0.0f, DXE_PI * 2.0f };
		std::uniform_real_distribution<f32> ddist{ 0.0f, DXE_PI * 0.5f };
		std::uniform_real_distribution<f32> odist{ 0.0f, DXE_PI * 0.08f };
		std::uniform_real_distribution<f32> rdist{ 6.0f,20.0f };
		std::uniform_real_distribution<f32> bdist{ 0.4f,3.0f };
		std::uniform_int_distribution<i32> latdist{ 5,20 };
		std::uniform_int_distribution<i32> longdist{ 10,40 };
		std::uniform_int_distribution<i32> typedist{ 0,2 };
	};
}
