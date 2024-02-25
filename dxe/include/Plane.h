#pragma once

#include "DXE.h"
#include "IndexedTriangleList.h"
#include "DXEMath.h"

namespace dxe
{
	class Plane
	{
	public:
		template<class V>
		static IndexedTriangleList<V> MakeTesselated(i32 divisions_x, i32 divisions_y)
		{
			namespace dx = DirectX;
			DXE_CHECK(divisions_x >= 1);
			DXE_CHECK(divisions_y >= 1);

			constexpr f32 width = 2.0f;
			constexpr f32 height = 2.0f;
			const i32 nVertices_x = divisions_x + 1;
			const i32 nVertices_y = divisions_y + 1;
			std::vector<V> vertices(nVertices_x * nVertices_y);

			{
				const f32 side_x = width / 2.0f;
				const f32 side_y = height / 2.0f;
				const f32 divisionSize_x = width / f32(divisions_x);
				const f32 divisionSize_y = height / f32(divisions_y);
				const auto bottomLeft = dx::XMVectorSet(-side_x, -side_y, 0.0f, 0.0f);

				for (i32 y = 0, i = 0; y < nVertices_y; y++)
				{
					const f32 y_pos = f32(y) * divisionSize_y;
					for (i32 x = 0; x < nVertices_x; x++, i++)
					{
						const auto v = dx::XMVectorAdd(
							bottomLeft,
							dx::XMVectorSet(f32(x) * divisionSize_x, y_pos, 0.0f, 0.0f)
						);
						dx::XMStoreFloat3(&vertices[i].pos, v);
					}
				}
			}

			std::vector<u16> indices;
			indices.reserve(sq(divisions_x * divisions_y) * 6);
			{
				const auto vxy2i = [nVertices_x](size_t x, size_t y)
					{
						return (u16)(y * nVertices_x + x);
					};
				for (size_t y = 0; y < divisions_y; y++)
				{
					for (size_t x = 0; x < divisions_x; x++)
					{
						const std::array<u16, 4> indexArray =
						{ vxy2i(x,y),vxy2i(x + 1,y),vxy2i(x,y + 1),vxy2i(x + 1,y + 1) };
						indices.push_back(indexArray[0]);
						indices.push_back(indexArray[2]);
						indices.push_back(indexArray[1]);
						indices.push_back(indexArray[1]);
						indices.push_back(indexArray[2]);
						indices.push_back(indexArray[3]);
					}
				}
			}

			return{ std::move(vertices),std::move(indices) };
		}
		template<class V>
		static IndexedTriangleList<V> Make()
		{
			return MakeTesselated<V>(1, 1);
		}
	};
}


