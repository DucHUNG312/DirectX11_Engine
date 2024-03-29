#include "Box.h"
#include "BindableBase.h"
#include "Cube.h"

namespace dxe
{
	Box::Box(Graphics& gfx,
		std::mt19937& rng,
		std::uniform_real_distribution<f32>& adist,
		std::uniform_real_distribution<f32>& ddist,
		std::uniform_real_distribution<f32>& odist,
		std::uniform_real_distribution<f32>& rdist,
		std::uniform_real_distribution<f32>& bdist)
		:
		r(rdist(rng)),
		droll(ddist(rng)),
		dpitch(ddist(rng)),
		dyaw(ddist(rng)),
		dphi(odist(rng)),
		dtheta(odist(rng)),
		dchi(odist(rng)),
		chi(adist(rng)),
		theta(adist(rng)),
		phi(adist(rng))
	{
		if (!IsStaticInitialized())
		{
			struct Vertex
			{
				dx::XMFLOAT3 pos;
			};
			auto model = Cube::Make<Vertex>();
			AddStaticBind(std::make_unique<VertexBuffer>(gfx, model.vertices));

			auto pvs = std::make_unique<VertexShader>(gfx, L"shadersout/ColorIndexVS.v.cso");
			auto pvsbc = pvs->GetBytecode();
			AddStaticBind(std::move(pvs));

			AddStaticBind(std::make_unique<PixelShader>(gfx, L"shadersout/ColorIndexPS.p.cso"));

			AddStaticBind(std::make_unique<IndexBuffer>(gfx, model.indices));

			struct PixelShaderConstants
			{
				struct
				{
					f32 r;
					f32 g;
					f32 b;
					f32 a;
				} face_colors[8];
			};
			const PixelShaderConstants cb2 =
			{
				{
					{ 1.0f,1.0f,1.0f },
					{ 1.0f,0.0f,0.0f },
					{ 0.0f,1.0f,0.0f },
					{ 1.0f,1.0f,0.0f },
					{ 0.0f,0.0f,1.0f },
					{ 1.0f,0.0f,1.0f },
					{ 0.0f,1.0f,1.0f },
					{ 0.0f,0.0f,0.0f },
				}
			};
			AddStaticBind(std::make_unique<PixelConstantBuffer<PixelShaderConstants>>(gfx, cb2));

			const std::vector<D3D11_INPUT_ELEMENT_DESC> ied =
			{
				{ "Position",0,DXGI_FORMAT_R32G32B32_FLOAT,0,0,D3D11_INPUT_PER_VERTEX_DATA,0 },
			};
			AddStaticBind(std::make_unique<InputLayout>(gfx, ied, pvsbc));

			AddStaticBind(std::make_unique<Topology>(gfx, D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST));
		}
		else
		{
			SetIndexFromStatic();
		}

		AddBind(std::make_unique<TransformCbuf>(gfx, *this));

		// model deformation transform (per instance, not stored as bind)
		dx::XMStoreFloat3x3(
			&mt,
			dx::XMMatrixScaling(1.0f, 1.0f, bdist(rng))
		);
	}

	void Box::Update(f32 dt) noexcept
	{
		roll += droll * dt;
		pitch += dpitch * dt;
		yaw += dyaw * dt;
		theta += dtheta * dt;
		phi += dphi * dt;
		chi += dchi * dt;
	}

	dx::XMMATRIX Box::GetTransformXM() const noexcept
	{
		return dx::XMLoadFloat3x3(&mt) *
			dx::XMMatrixRotationRollPitchYaw(pitch, yaw, roll) *
			dx::XMMatrixTranslation(r, 0.0f, 0.0f) *
			dx::XMMatrixRotationRollPitchYaw(theta, phi, chi) *
			dx::XMMatrixTranslation(0.0f, 0.0f, 20.0f);
	}
}
