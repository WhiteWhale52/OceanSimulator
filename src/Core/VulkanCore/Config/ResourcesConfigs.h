#pragma once
#include "CommonHeaders.h"

namespace Vulkan::Config {
	struct ImageConfig {
		uint32_t width = 1;
        uint32_t    height = 1;
        uint32_t    depth = 1;
        uint32_t    mipLevels = 1;
        uint32_t    layers = 1;
        vk::Format format= vk::Format::eR8G8B8A8Unorm;
        vk::ImageUsageFlagBits  usage = vk::ImageUsageFlagBits::eSampled;
        bool        isCubemap = false;
        const char* debugName = nullptr;
	};

    enum class MemoryAccess {
        GPUOnly,      // Device local, no CPU access
        CPUToGPU,     // Persistently mapped, CPU writes each frame
        GPUToCPU,     // Readback
    };

    struct BufferConfig {
        vk::DeviceSize size = 0;
        vk::BufferUsageFlagBits2 usage = vk::BufferUsageFlagBits2::eVertexBuffer;
        MemoryAccess access = MemoryAccess::GPUOnly;
        bool mapped = false;
        const char* debugName = nullptr;
    };
}
