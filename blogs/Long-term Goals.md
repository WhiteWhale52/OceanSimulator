# Long-term goals and features

- [ ] Integrate ECS, perhaps?? I am going to use [entt](https://github.com/skypjack/entt) to do that.

- [ ] Integrate a profiler, perhaps ?? I am thinking of using [optick: C++ Profiler For Games](https://github.com/bombomby/optick)

- [ ] Integrate clang-tidy to enforce code style

- [ ] Create automated tests 

- [ ] I need lighting, more details will be provided later.

### Layers

- [ ] Separate the code into layers
  
  * Core Layers: Includes
    
    * Instance creation
    
    * Debugs and Validation
    
    * Surface Creation (We are using GLFW so here is where we create the GLFW window )
    
    * Physical Device
    
    * Getting the Queue Families from the Physical Device
    
    * Creating a Logical Device, LDevice, based on the Queue
    
    * Memory Allocation using VMA, more details on VMA below
    
    * Buffers
    
    * Images
    
    * Command Pools
    
    * Job System
    
    * Synchronization
    
    * Shader Module
    
    * Descriptor Pools and Layouts
    
    * Pipeline (Details below)
  
  * Renderer Layer: Calls the Vulkan function, it is responsible over dispatching commands to buffers 
    
    * Swapchain management by creating, using, and destroying Swapchain
    
    * Renderer Passes
    
    * FrameBuffers
    
    * Pipeline Creation
    
    * Descriptor binding during command recording 
    
    * Queue Submission
    
    * Samplers
    
    * Dispatching the vkFFT commands to get the FFT data
    
    * RUNS EVERYTHING ON A SEPARATE RENDER THREAD WHICH IS CREATED USING THE JOB SYSTEM (more on that in another note)
  
  * Engine Layer: This holds no Vulkan function
    
    * Produces an EngineFrame, which contains the OceanParams, these are the coefficients that we pass into the vkFFT calls and into the Tessendorf and Horvath equations
    
    * The **Renderer ** consumes the EngineFrame
    
    * 
  
  * Lastly, the Application Layer 
    
    * Runs the main loop
      
      * This involves the timestep logic
      
      * Pausing
      
      * Toggling simulation speed
    
    * Thread management
      
      * Starts the render thread
      
      * Creates communication queues, not Vulkan, between threads

- [ ] The main loop of the Application layer is the most superficial class, it is the entry to this application.
  
  * As such, we need to 
    
    * Handle Inputs
    
    * Update Time
    
    * Render the frame
  
  * We need to **implement different Timing mechanisms**
  
  * Handle inputs by polling events every iteration 
    
    

### Learning Concepts

- [ ] Learn about CMake and how to use it with Visual Studio

- [ ] Learn about std::threads

- [ ] Learn about std concurrency

- [ ] Read about Lock-free DS, there are several resources found

- [ ] Read the memory management chapter of Vulkan Spec

- [ ] Read about Resource Managment

- [ ] Read about creating a Job System for the Engine layer

- [ ] 
