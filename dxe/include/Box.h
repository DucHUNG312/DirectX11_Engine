#pragma once

#include "DXE.h"
#include "DrawableBase.h"

namespace dxe
{
	class Box : public DrawableBase<Box>
	{
	public:
		Box(Graphics& gfx, std::mt19937& rng,
			std::uniform_real_distribution<f32>& adist,
			std::uniform_real_distribution<f32>& ddist,
			std::uniform_real_distribution<f32>& odist,
			std::uniform_real_distribution<f32>& rdist,
			std::uniform_real_distribution<f32>& bdist);
		void Update(f32 dt) noexcept override;
		dx::XMMATRIX GetTransformXM() const noexcept override;
	private:
		// positional
		f32 r;
		f32 roll = 0.0f;
		f32 pitch = 0.0f;
		f32 yaw = 0.0f;
		f32 theta;
		f32 phi;
		f32 chi;
		// speed (delta/s)
		f32 droll;
		f32 dpitch;
		f32 dyaw;
		f32 dtheta;
		f32 dphi;
		f32 dchi;
		// model transform
		dx::XMFLOAT3X3 mt;
	};
}
