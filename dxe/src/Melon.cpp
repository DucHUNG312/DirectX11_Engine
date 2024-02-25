#include "Melon.h"
#include "Sphere.h"
#include "BindableBase.h"

namespace dxe
{
	Melon::Melon(Graphics& gfx,
		std::mt19937& rng,
		std::uniform_real_distribution<f32>& adist,
		std::uniform_real_distribution<f32>& ddist,
		std::uniform_real_distribution<f32>& odist,
		std::uniform_real_distribution<f32>& rdist,
		std::uniform_int_distribution<i32>& longdist,
		std::uniform_int_distribution<i32>& latdist)
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
			auto pvs = std::make_unique<VertexShader>(gfx, L"shadersout/ColorIndexVS.v.cso");
			auto pvsbc = pvs->GetBytecode();
			AddStaticBind(std::move(pvs));

			AddStaticBind(std::make_unique<PixelShader>(gfx, L"shadersout/ColorIndexPS.p.cso"));

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

		struct Vertex
		{
			dx::XMFLOAT3 pos;
		};
		auto model = Sphere::MakeTesselated<Vertex>(latdist(rng), longdist(rng));
		// deform vertices of model by linear transformation
		model.Transform(dx::XMMatrixScaling(1.0f, 1.0f, 1.2f));

		AddBind(std::make_unique<VertexBuffer>(gfx, model.vertices));

		AddBind(std::make_unique<IndexBuffer>(gfx, model.indices));

		AddBind(std::make_unique<TransformCbuf>(gfx, *this));
	}

	void Melon::Update(f32 dt) noexcept
	{
		roll += droll * dt;
		pitch += dpitch * dt;
		yaw += dyaw * dt;
		theta += dtheta * dt;
		phi += dphi * dt;
		chi += dchi * dt;
	}

	DirectX::XMMATRIX Melon::GetTransformXM() const noexcept
	{
		return dx::XMMatrixRotationRollPitchYaw(pitch, yaw, roll) *
			dx::XMMatrixTranslation(r, 0.0f, 0.0f) *
			dx::XMMatrixRotationRollPitchYaw(theta, phi, chi) *
			dx::XMMatrixTranslation(0.0f, 0.0f, 20.0f);
	}
}
