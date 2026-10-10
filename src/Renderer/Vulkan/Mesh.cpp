#include "Mesh.h"

namespace TheRenderer::Vulkan {
	MeshData GenerateMesh(uint32_t cells, float size) {
		MeshData meshData;

		const uint32_t vertsPerSide = cells + 1;

		meshData.vertices.reserve(vertsPerSide * vertsPerSide);
		for (uint32_t row = 0; row < vertsPerSide; row++)
		{
			for (uint32_t col = 0; col < vertsPerSide; col++) {
				float u = static_cast<float>(col) / cells;
				float v = static_cast<float>(row) / cells;

				Vertex vert;

				vert.position = { (u - 0.5f) * size, (v - 0.5f) * size, 0.0f };
				vert.colour = { u,v,1.0f - u, 1.0f };
				meshData.vertices.push_back(vert);

			}
		}

		meshData.indices.reserve(cells * cells * 6);
		for (uint32_t row = 0; row < cells; row++)
		{
			for (uint32_t col = 0; col < cells; col++) {
				uint32_t topLeft = row * vertsPerSide + col;
				uint32_t topRight = topLeft + 1;
				uint32_t bottomLeft = topLeft + vertsPerSide;
				uint32_t bottomRight = bottomLeft + 1;

				meshData.indices.push_back(topLeft);
				meshData.indices.push_back(bottomLeft);
				meshData.indices.push_back(topRight);

				meshData.indices.push_back(topRight);
				meshData.indices.push_back(bottomLeft);
				meshData.indices.push_back(bottomRight);
			}
		}
		return meshData;
	}
}