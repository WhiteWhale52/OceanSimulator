#pragma once

#include "config.h"


bool supports(
	const vk::PhysicalDevice& physicalDevice,
	const char** ppRequestedExtensions,
	const uint32_t requestedExtensionCount
);

/// <summary>
/// Checks whether this physical device is suitable for the application
/// </summary>
/// <param name="physicalDevice">The physical device we are checking</param>
/// <returns>
/// true if the physical device is suitable
/// fakse if the physical device lacks at least one of the requested extensions
/// </returns>
bool is_suitable(const vk::PhysicalDevice& physicalDevice);


/// <summary>
/// Chooses the physical device
/// </summary>
/// <param name="instance">
/// Vulkan instance which will use the device</param>
/// <returns>
/// vk::PhysicalDevice the selected physical device
/// </returns>
vk::PhysicalDevice choose_physical_device(
	const vk::Instance& instance
);