#### Day 3:

* This is the first day of code writing
  
  

##### Tasks for the day

1. Start the Vulkan initialization; Here is a short checklist
   [ ]  Create `vk::Instance` 
   [ ] Validation layers.
   [ ]  Pick vk::PhysicalDevice with VK_QUEUE_GRAPHICS_BIT and VK_QUEUE_COMPUTE_BIT support (prefer one with discrete GPU).
   [ ] Create vk::Device and grab vk::Queue handles for graphics and compute.
   [ ]  Create 2 vk::CommandPools: one for graphics, one for compute (or share if same queue family).
   [ ] Create vk::DescriptorPool and layout(s) for compute/graphics descriptors.

2. Make the PlantUML Diagram showing all the steps for each stage of the project
   
   * Making the base mesh
   
   * How IFFT, in particular, VkFFT will be added
   
   * How IFFT output will be added to the vkSurface of GLFW 
* There is a difference between ImGUI and GLFW 
  
  * GLFW handles all the OS-specific details since Vulkan is platform-agnostic and so it doesn't know how to create windows or connect to the OS's display system; The plan is to make this project buildable and executable on different OS systems with difference window systems like Win32, Xlib, Wayland
  
  * GLFW needs some vulkan extensions which I will enable using glfwGetRequiredInstanceExtenstions(&count); which would return an array of strings that are the extensitions that GLFW depend on when creating a Vulkan surface
  
  * Then in the instance create info struct we will enable these extenstions
  
  * On the other hand, ImGUI doesn't have/require its own extensitons it only depends on GLFW

* Details about Vulkan API
  
  * There are several keywords or concepts that I need to remember and know
    
    * Application: This is the entire application not just the vulkan portion so if the application is a game engine it would be complex and will contain a physics engine portion, a saving system portion, an input/output portion, and finally a rendering portion which is the vulkan object
    
    * vk::instance: This is the connection between the application and the vulkan runtime/driver. It is responsible over a lot of stuff including but not limited to, 
      
      * load the Vulkan loader, 
      
      * know the app like version, name, engine info
      
      * manage the Vulkan global state
      
      * enumerate all the Physical Device (GPUs)
      
      * load instance-level extensions like surface creation and validation layer
      
      * vk::PhysicalDevice: The actual physical GPU; 
        
        * Note that Vulkan has a PhysicalDevice and a LogicalDevice
      
      * vk::device : The logical device is the software connection to the physical device (GPU); It is created from a Physical device and is responsible over creating buffers, images, pipeline, accessing the GPU queues etc
      
      * vk::queue : Each physical device supports several queue families like Graphics Queue, Compute Queue, Transfer Queue
      
      * vk::commandpool : This holds command buffer instances which each record GPU commands
      
      * vk::SurfaceKHR : This represents the window we are rendering to in Vulkan, so we create a vk::SurfaceKHR instance and call glfwCreateWindowSurface and pass the surface by reference;
        
        * Each surface contains a swapchain attached to that surface, each which hold images that are shown on the screen one at a time
      
      * vk::image : Mentioned above. Each vk::image is a raw texture so to use it as a render target, we wrap it in an image view which specify : viewType, format, aspectMask, reading format/ access 
      
      * vk::framebuffer : For each vk::image we create a framebuffer that ties that image view to the render pass
      
      * The render loop: This consists of several stages including: **Acquiring an image; Recording Command Buffer; Submit to GPU; Present to the Window** (https://vkguide.dev/docs/new_chapter_1/vulkan_mainloop/)
  
  #### End of the day:
  
  * I wasn't able to progress as much as I wanted to, but I 
    
    * Made the CMakeLists.txt, 
    
    * Added vkFFT to the project
    
    * Configured an appropriate cmake version
    
    * Studied the different concepts in vulkan and watched some videos explaining vkFFT, I have implemented a Vulkan project before am quite familiar with several of the concepts and I still actively learning and reading the documentation extensively


