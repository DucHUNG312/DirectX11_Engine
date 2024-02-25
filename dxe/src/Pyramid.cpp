#include "Pyramid.h"
#include "Cone.h"
#include "BindableBase.h"

namespace dxe
{
	Pyramid::Pyramid(Graphics& gfx,
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
				struct
				{
					unsigned char r;
					unsigned char g;
					unsigned char b;
					unsigned char a;
				} color;
			};
			auto model = Cone::MakeTesselated<Vertex>(4);
			// set vertex colors for mesh
			model.vertices[0].color = { 255,255,0 };
			model.vertices[1].color = { 255,255,0 };
			model.vertices[2].color = { 255,255,0 };
			model.vertices[3].color = { 255,255,0 };
			model.vertices[4].color = { 255,255,80 };
			model.vertices[5].color = { 255,10,0 };
			// deform mesh linearly
			model.Transform(dx::XMMatrixScaling(1.0f, 1.0f, 0.7f));

			AddStaticBind(std::make_unique<VertexBuffer>(gfx, model.vertices));

			auto pvs = std::make_unique<VertexShader>(gfx, L"shadersout/ColorBlendVS.v.cso");
			auto pvsbc = pvs->GetBytecode();
			AddStaticBind(std::move(pvs));

			AddStaticBind(std::make_unique<PixelShader>(gfx, L"shadersout/ColorBlendPS.p.cso"));

			AddStaticBind(std::make_unique<IndexBuffer>(gfx, model.indices));

			const std::vector<D3D11_INPUT_ELEMENT_DESC> ied =
			{
				{ "Position",0,DXGI_FORMAT_R32G32B32_FLOAT,0,0,D3D11_INPUT_PER_VERTEX_DATA,0 },
				{ "Color",0,DXGI_FORMAT_R8G8B8A8_UNORM,0,12,D3D11_INPUT_PER_VERTEX_DATA,0 },
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

	void Pyramid::Update(f32 dt) noexcept
	{
		roll += droll * dt;
		pitch += dpitch * dt;
		yaw += dyaw * dt;
		theta += dtheta * dt;
		phi += dphi * dt;
		chi += dchi * dt;
	}

	DirectX::XMMATRIX Pyramid::GetTransformXM() const noexcept
	{
		return dx::XMMatrixRotationRollPitchYaw(pitch, yaw, roll) *
			dx::XMMatrixTranslation(r, 0.0f, 0.0f) *
			dx::XMMatrixRotationRollPitchYaw(theta, phi, chi) *
			dx::XMMatrixTranslation(0.0f, 0.0f, 20.0f);
	}
}
