#pragma once

#include "DXE.h"
#include "IndexedTriangleList.h"
#include "DXEMath.h"

namespace dxe
{
	class Sphere
	{
	public:
		template<class V>
		static IndexedTriangleList<V> MakeTesselated(i32 latDiv, i32 longDiv)
		{
			namespace dx = DirectX;
			DXE_CHECK(latDiv >= 3);
			DXE_CHECK(longDiv >= 3);

			constexpr f32 radius = 1.0f;
			const auto base = dx::XMVectorSet(0.0f, 0.0f, radius, 0.0f);
			const f32 lattitudeAngle = DXE_PI / latDiv;
			const f32 longitudeAngle = 2.0f * DXE_PI / longDiv;

			std::vector<V> vertices;
			for (i32 iLat = 1; iLat < latDiv; iLat++)
			{
				const auto latBase = dx::XMVector3Transform(
					base,
					dx::XMMatrixRotationX(lattitudeAngle * iLat)
				);
				for (i32 iLong = 0; iLong < longDiv; iLong++)
				{
					vertices.emplace_back();
					auto v = dx::XMVector3Transform(
						latBase,
						dx::XMMatrixRotationZ(longitudeAngle * iLong)
					);
					dx::XMStoreFloat3(&vertices.back().pos, v);
				}
			}

			// add the cap vertices
			const auto iNorthPole = (u16)vertices.size();
			vertices.emplace_back();
			dx::XMStoreFloat3(&vertices.back().pos, base);
			const auto iSouthPole = (u16)vertices.size();
			vertices.emplace_back();
			dx::XMStoreFloat3(&vertices.back().pos, dx::XMVectorNegate(base));

			const auto calcIdx = [latDiv, longDiv](u16 iLat, u16 iLong)
				{ return iLat * longDiv + iLong; };
			std::vector<u16> indices;
			for (u16 iLat = 0; iLat < latDiv - 2; iLat++)
			{
				for (u16 iLong = 0; iLong < longDiv - 1; iLong++)
				{
					indices.push_back(calcIdx(iLat, iLong));
					indices.push_back(calcIdx(iLat + 1, iLong));
					indices.push_back(calcIdx(iLat, iLong + 1));
					indices.push_back(calcIdx(iLat, iLong + 1));
					indices.push_back(calcIdx(iLat + 1, iLong));
					indices.push_back(calcIdx(iLat + 1, iLong + 1));
				}
				// wrap band
				indices.push_back(calcIdx(iLat, longDiv - 1));
				indices.push_back(calcIdx(iLat + 1, longDiv - 1));
				indices.push_back(calcIdx(iLat, 0));
				indices.push_back(calcIdx(iLat, 0));
				indices.push_back(calcIdx(iLat + 1, longDiv - 1));
				indices.push_back(calcIdx(iLat + 1, 0));
			}

			// cap fans
			for (u16 iLong = 0; iLong < longDiv - 1; iLong++)
			{
				// north
				indices.push_back(iNorthPole);
				indices.push_back(calcIdx(0, iLong));
				indices.push_back(calcIdx(0, iLong + 1));
				// south
				indices.push_back(calcIdx(latDiv - 2, iLong + 1));
				indices.push_back(calcIdx(latDiv - 2, iLong));
				indices.push_back(iSouthPole);
			}
			// wrap triangles
			// north
			indices.push_back(iNorthPole);
			indices.push_back(calcIdx(0, longDiv - 1));
			indices.push_back(calcIdx(0, 0));
			// south
			indices.push_back(calcIdx(latDiv - 2, 0));
			indices.push_back(calcIdx(latDiv - 2, longDiv - 1));
			indices.push_back(iSouthPole);

			return { std::move(vertices),std::move(indices) };
		}
		template<class V>
		static IndexedTriangleList<V> Make()
		{
			return MakeTesselated<V>(12, 24);
		}
	};
}
