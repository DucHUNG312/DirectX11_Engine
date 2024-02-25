#pragma once

#include "DXE.h"

namespace dxe
{
	template<class T>
	class IndexedTriangleList
	{
	public:
		IndexedTriangleList() = default;
		IndexedTriangleList(std::vector<T> verts_in, std::vector<u16> indices_in)
			:
			vertices(std::move(verts_in)),
			indices(std::move(indices_in))
		{
			DXE_CHECK(vertices.size() > 2);
			DXE_CHECK(indices.size() % 3 == 0);
		}
		void Transform(dx::FXMMATRIX matrix)
		{
			for (auto& v : vertices)
			{
				const dx::XMVECTOR pos = dx::XMLoadFloat3(&v.pos);
				dx::XMStoreFloat3(
					&v.pos,
					dx::XMVector3Transform(pos, matrix)
				);
			}
		}

	public:
		std::vector<T> vertices;
		std::vector<u16> indices;
	};
}
