#pragma once
#include "Vertex.h"

namespace TheRenderer::Vulkan {
    const std::vector<Vertex> triangleVertices = {
      { { 0.0f, -0.5f, 0.0f },{ 1.0f, 0.0f, 0.0f, 1.0f } },
      { { 0.5f,  0.5f, 0.0f },{ 0.0f, 1.0f, 0.0f, 1.0f } },
      { { -0.5f,  0.5f, 0.0f },{ 0.0f, 0.0f, 1.0f, 1.0f } },
    };
}

