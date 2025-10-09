#### Day 3:

* This is the first day of code writing
  
  ##### Tasks for the day
  
  1. Start the Vulkan initialization; Here is a short checklist
     [ ]  Create `vk::Instance` (+ validation layers while developing).
     [ ]  Pick vk::PhysicalDevice with VK_QUEUE_GRAPHICS_BIT and VK_QUEUE_COMPUTE_BIT support (prefer one with discrete GPU).
     [ ] Create vk::Device and grab vk::Queue handles for graphics and compute.
     
     [ ]  Create 2 vk::CommandPools: one for graphics, one for compute (or share if same queue family).
     
     [ ] Create vk::DescriptorPool and layout(s) for compute/graphics descriptors.
  
  2. Make the PlantUML Diagram showing all the steps for each stage of the project
     
     * Making the base mesh
     
     * How IFFT, in particular, VkFFT will be added
     
     * How IFFT output will be added to the vkSurface of GLFW 
     
     
     
     
