#include "Box.h"
#include "BindableBase.h"
#include "Sphere.h"

namespace dxe
{
	Box::Box(Graphics& gfx,
		std::mt19937& rng,
		std::uniform_real_distribution<f32>& adist,
		std::uniform_real_distribution<f32>& ddist,
		std::uniform_real_distribution<f32>& odist,
		std::uniform_real_distribution<f32>& rdist)
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
			auto model = Sphere::Make<Vertex>();
			model.Transform(dx::XMMatrixScaling(1.0f, 1.0f, 1.2f));
			AddStaticBind(std::make_unique<VertexBuffer>(gfx, model.vertices));

			auto pvs = std::make_unique<VertexShader>(gfx, L"shadersout/VertexShader.v.cso");
			auto pvsbc = pvs->GetBytecode();
			AddStaticBind(std::move(pvs));

			AddStaticBind(std::make_unique<PixelShader>(gfx, L"shadersout/PixelShader.p.cso"));

			AddStaticBind(std::make_unique<IndexBuffer>(gfx, model.indices));

			struct ConstantBuffer2
			{
				struct
				{
					f32 r;
					f32 g;
					f32 b;
					f32 a;
				} face_colors[6];
			};
			const ConstantBuffer2 cb2 =
			{
				{
					{ 1.0f,0.0f,1.0f },
					{ 1.0f,0.0f,0.0f },
					{ 0.0f,1.0f,0.0f },
					{ 0.0f,0.0f,1.0f },
					{ 1.0f,1.0f,0.0f },
					{ 0.0f,1.0f,1.0f },
				}
			};
			AddStaticBind(std::make_unique<PixelConstantBuffer<ConstantBuffer2>>(gfx, cb2));

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
		return dx::XMMatrixRotationRollPitchYaw(pitch, yaw, roll) *
			dx::XMMatrixTranslation(r, 0.0f, 0.0f) *
			dx::XMMatrixRotationRollPitchYaw(theta, phi, chi) *
			dx::XMMatrixTranslation(0.0f, 0.0f, 20.0f);
	}
}
