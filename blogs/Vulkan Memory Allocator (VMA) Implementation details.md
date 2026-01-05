# Vulkan Memory Allocator (VMA) Implementation details

## Memory in Vulkan

Vulkan contains memory heaps and memory types, both of which get exposed by calling `vkPhysicalDeviceMemoryProperties`. The memory heaps are big pools of bytes while the memory types are a combination of heap index and property flags like HOST_VISIBLE, DEVICE_LOCAL, HOST_COHERENT, etc. 

Therefore, so use memory we must always specify the memory-type and then allocate it.

Each allocation takes some space in the memory heap. 

Since we manually allocate the bytes in the heap, we must know what the memory allignment, size and memorytypeBits are which we get by calling vkGet*MemoryRequirements() telling us which memory type indices are valid for this resource

There are several memory properties. Some of which determine how memory could be accessed,

* `VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT`: fastest for GPU (often VRAM), may **not** be host-visible.

* `VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT`: CPU can map with `vkMapMemory`.

* `VK_MEMORY_PROPERTY_HOST_COHERENT_BIT`: removes need for explicit flush/invalidate (otherwise you must call `vkFlushMappedMemoryRanges` / `vkInvalidateMappedMemoryRanges` or use helper calls).

* `VK_MEMORY_PROPERTY_HOST_CACHED_BIT`: affects read/write caching semantics. 
  These flags drive your pattern (GPU-only, staging, readback, dynamic CPU updates).

## What VMA does?

VMA automates the Vulkan memory-type selection and suballocation, it implements pooliung, deframentation helpers, mapping from GPU memory to CPU, convience helpers for creating, allocating and binding buffers in one calls. 

So, VMA sells us convience and more control over memory.

E.g. Calling `vmaCreateBuffer` creates the resource, the buffer, allocates its memory in the memory heap and binds it all in one call.

VMA also gives us budget info, which is the per-heap usage.

## How to use it?

Here is the real-part of this note, we must 

* Create a VMA allocator which takes in the vkInstance, vkPhysicalDevice, and vkLogicalDevice

* In the VMA allocator, we create an allocatorCreateInfo struct and fill it in with data.

* We need to write a function for each type of allocation we want, e.g. if we want to create a device-local buffer we need to create a function that takes in the allocator and the bufferUsageFlags and returns the buffer and allocation details.


