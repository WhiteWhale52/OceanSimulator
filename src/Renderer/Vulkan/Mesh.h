#pragma once

#include "Vertex.h"

namespace TheRenderer::Vulkan {
	struct MeshData {
		std::vector<Vertex> vertices;
		std::vector<uint32_t> indices;
	};

	// cells = quads per side, size = total width/height
	MeshData GenerateMesh(uint32_t cells, float size);
}