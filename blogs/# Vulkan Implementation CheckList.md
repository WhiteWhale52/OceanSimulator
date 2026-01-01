# Vulkan Implementation CheckList

### Foundation / Boilerplate

1. Instance & Validation

------------------------

**Goal:** Talk to the Vulkan loader and enable debugging.

* [ ] Create `VkInstance`

* [ ] Choose extensions:
  
  * `VK_KHR_surface`
  
  * Platform surface extension (`VK_KHR_win32_surface`, `VK_KHR_xcb_surface`, etc.)

* [ ] Enable validation layers (`VK_LAYER_KHRONOS_validation`)

* [ ] Setup debug messenger (`vkCreateDebugUtilsMessengerEXT`)

* [ ] Query available layers/extensions

🔑 Concept:

> Instance = connection between your app and Vulkan runtime

* * *

2. Physical Device Selection (GPU)

----------------------------------

**Goal:** Pick which GPU you will use.

- [ ] Enumerate physical devices

- [ ] Query:
  
  * Queue families
  
  * Features
  
  * Limits
  
  * Memory types

- [ ] Choose:
  
  * Graphics queue
  
  * Present queue
  
  * Compute queue (optional)

* * *

3. Logical Device & Queues

--------------------------

**Goal:** Create the actual device you submit work to.

* [ ] Create `VkDevice`

* [ ] Enable features (e.g. samplerAnisotropy)

* [ ] Request queues from queue families

* [ ] Retrieve queue handles

* * *

### PHASE 1 — Presentation (Swapchain)

4. Window Surface

-----------------

* [ ] Create OS-specific surface

* [ ] Query surface capabilities
5. Swapchain

------------

* [ ] Choose:
  
  * Format (e.g. `VK_FORMAT_B8G8R8A8_SRGB`)
  
  * Present mode (FIFO / MAILBOX)
  
  * Image count

* [ ] Create swapchain

* [ ] Get swapchain images

* [ ] Create image views

🔑 Swapchain = images you render into that the OS presents

* * *

### PHASE 2 — Synchronization (CRITICAL)

6. Synchronization Primitives

-----------------------------

You must manage **everything**.

* [ ] Fences (CPU ↔ GPU)

* [ ] Semaphores (GPU ↔ GPU)

* [ ] Timeline semaphores (optional but powerful)

* [ ] Pipeline barriers

* [ ] Image layout transitions

* [ ] Memory barriers

🔥 This is where most Vulkan bugs live.

* * *

### PHASE 3 — Memory Management

7. GPU Memory

-------------

* Allocate memory **using VMA**

* Bind memory to buffers/images **using VMA**

Need to understand:

* [ ] Memory types (HOST_VISIBLE, DEVICE_LOCAL)

* [ ] Coherent vs non-coherent

* [ ] Staging buffers

* [ ] Mapping and flushing

* * *

### PHASE 4 — Resources

8. Buffers

----------

* [ ] Vertex buffers

* [ ] Index buffers

* [ ] Uniform buffers

* [ ] Storage buffers

* [ ] Indirect buffers

Each requires:

* Creation

* Memory allocation

* Binding

* Descriptor binding
9. Images & Textures

--------------------

* Image creation

* Image layout transitions

* Image views

* Samplers

* Mipmaps

* Format selection

* * *

### PHASE 5 — Descriptors (Binding Resources)

10. Descriptor Sets & Layouts

-----------------------------

* [ ] Descriptor set layouts

* [ ] Descriptor pools

* [ ] Descriptor sets

* [ ] Updating descriptors

Types:

* Uniform buffer

* Storage buffer

* Combined image sampler

* Storage image

* Texel buffer

This replaces “binding textures and uniforms” from OpenGL.

* * *

### PHASE 6 — Pipelines (The Big One)

11. Shader Modules

------------------

* [ ] Compile GLSL → SPIR-V

* [ ] Load SPIR-V binaries
- [ ] Automate that process with a .bat file for Windows and .bash for Linux/MacOS
12. Pipeline Layout

-------------------

* [ ] Descriptor set layouts

* [ ] Push constants
13. Graphics Pipeline

---------------------

Includes:

* Vertex input layout

* Input assembly

* Viewport/scissor

* Rasterizer

* Multisampling

* Depth/stencil

* Color blending

* Render pass compatibility

💡 Pipelines are immutable and expensive to create.

* * *

### PHASE 7 — Render Pass System

14. Render Pass

---------------

Defines:

* Attachments (color, depth)

* Load/store operations

* Subpasses

* Dependencies
15. Framebuffers

----------------

* One framebuffer per swapchain image

* * *

### PHASE 8 — Command Buffers

16. Command Pools

-----------------

* One per thread typically
17. Command Buffers

-------------------

You must record:

* Begin render pass

* Bind pipeline

* Bind descriptors

* Bind vertex/index buffers

* Draw calls

* End render pass

Command buffers are immutable once submitted.

* * *

### PHASE 9 — Frame Loop

18. Frame Lifecycle

-------------------

Typical loop:

1. Wait for fence

2. Acquire swapchain image

3. Record command buffer

4. Submit to queue

5. Present image

Includes:

* Frame-in-flight system (double/triple buffering)

* Per-frame resources

* * *

### PHASE 10 — Advanced Rendering

19. Compute Pipelines

---------------------

* Separate pipeline type

* Dispatch calls

* Used for:
  
  * Culling
  
  * Lighting
  
  * VkFFT Calculations
20. Indirect Rendering
* Indirect draw buffers

* GPU-generated draw commands
21. GPU-Driven Rendering

------------------------



### PHASE 11 — Advanced Memory & Performance

22. Resource Transitions
    
    * * *
    * [ ] Image layouts
    
    * [ ] Ownership transfers
    
    * [ ] Queue family ownership
23. Descriptor Indexing (Bindless)

----------------------------------

* [ ] Large descriptor arrays

* [ ] Dynamic indexing in shaders

* * *

### 
