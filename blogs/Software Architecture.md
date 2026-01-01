## Using a software architecture

#### The motivation behind using a clean software architecture is separation of concerns of each submodule in the project.

#### We organise the folder/files into the Core, Engine, Renderer, and Application Layers

* Core: The core layer is where the low-level Vulkan setup and platform-agnostic utils lives. So, this is where these files should live
  
  * PhysicalDevice.cpp/.h
  
  * QueueFamilies.cpp/.h
  
  * VulkanInstance.cpp/.h
  
  * Math, Timing, and Logging libraries
  
  This layer should be thought of as the foundation of the entire application.

* Renderer Layer: This is where rendering and compute logic is built on top of the Vulkan abstractions. This means that it is a great place to store
  
  * Command buffers, descriptor sets, pipelines.  
  * Frame rendering loop. 
  * GPU FFT compute passes (binding buffers, dispatch calls, etc.).
  * Anything that records GPU work.
  
  Renderer Layer will use the Core Layer Vulkan wrappers, like PhysicalDevice, QueueFamilies, etc to request queues from the queue families, allocate command buffers, and submit work to queues for rendering.

* Engine Layer: This layer shouldn't care about Vulkan at all, it deals with Math and Simulation Logic. This is where each live
  
  * FFT Logic
  
  * JONSWAP spectra generation
  
  * Heightfield updates, wind and wave dispersion relations
  
  This layer can produce data (like a buffer of Fourier coefficients) and the Renderer Layer consumes it for visualization
  
  

### Because of all of this abstraction, I will need to have a CMake target per layer

```objectivec

/ FluidSimulator
│
├── Core
│   ├── CMakeLists.txt
│   ├── Vulkan/
│   │   ├── PhysicalDevice.cpp
│   │   ├── QueueFamilies.cpp
│   │   └── VulkanInstance.cpp
│   ├── Math/
│   │   ├── Vector.h
│   │   └── Matrix.h
│   └── Logging.cpp
│
├── Renderer/
│   ├── CMakeLists.txt
│   ├── RenderGraph.cpp
│   ├── CommandBuffer.cpp
│   └── OceanFFTComputePass.cpp
│
├── Engine/
│   ├── CMakeLists.txt
│   ├── OceanSimulation.cpp
│   └── FFTLogic.cpp
│
└── App/
    ├── CMakeLists.txt
    └── main.cpp
```

  And in the top level CMakeLists.txt file we add each layer, with the exception of the app layer, as a subdirectory.

  In each of the layers, we can link the files of the layer to the target as STATIC. 

  E.g. For the Core Layer

```cmake
add_library(Core STATIC
    Vulkan/PhysicalDevice.cpp
    Vulkan/QueueFamilies.cpp
    Vulkan/VulkanInstance.cpp
    Math/Vector.cpp
    Logging.cpp
)

target_include_directories(Core PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
find_package(Vulkan REQUIRED)
target_link_libraries(Core PUBLIC Vulkan::Vulkan)
```



In the App layer CMakeLists.txt file we link each of the sublayers 

```cmake
add_executable(App main.cpp)

target_link_libraries(App PRIVATE Engine Renderer Core)
```



Also, just to make sure that I don't forget this, FFT logic should live in two layers, Engine and Renderer.

In the Enigne Layer, we include the mathematical-side FFT like 

* Generating h₀(k), h(k, t)

* Applying dispersion relations

* Managing ocean spectra

In the Renderer Layer, we include the GPU FFT like

* Dispatching FFT compute shaders

* Managing ping-pong buffers

* Integrating with render graph / command buffers
  
  

## File Organisation

src/ should become the root for submodules like Core/, Engine/, Renderer/,  and App/. It separate the code logically into modules

```objectivec
src/
├── Core/
│   ├── Vulkan/
│   │   ├── PhysicalDevice.cpp/.h
│   │   ├── QueueFamilies.cpp/.h
│   │   ├── VulkanInstance.cpp/.h
│   │   └── VulkanUtils.h
│   ├── Math/
│   │   ├── Vector.h
│   │   └── Matrix.h
│   ├── Logging.cpp/.h
│   └── CMakeLists.txt
│
├── Renderer/
│   ├── OceanFFTComputePass.cpp/.h
│   ├── CommandBuffer.cpp/.h
│   ├── RenderGraph.cpp/.h
│   └── CMakeLists.txt
│
├── Engine/
│   ├── OceanSimulation.cpp/.h
│   ├── FFTLogic.cpp/.h
│   └── CMakeLists.txt
│
└── App/
    ├── main.cpp
    ├── SandboxScene.cpp/.h
    └── CMakeLists.txt


```



The root CMakeLists.txt file should include these lines

```cmake
add_subdirectory(src/Core)
add_subdirectory(src/Renderer)
add_subdirectory(src/Engine)
add_subdirectory(src/App)
```



Since the Core Layer is small, foundational and, rarely changes during runtime. We can compile it as a static library and link it with the others.

The App Layer, on the other hand, should be an executable that links all of the other layers.



Communication Between Threads

I'll need a **synchronization system** — typically a **double-buffered queue** or **lock-free ring buffer** between my**Engine** and **Renderer** layers.

### Example structure:

```cpp
struct FrameData {
    SceneSnapshot snapshot;
    Camera camera;
    OceanFFTState fftState;
};

class RenderQueue {
public:
    void Push(const FrameData& data);  // called by engine thread
    bool Pop(FrameData& out);          // called by render thread
private:
    std::atomic<uint32_t> head, tail;
    FrameData frames[2]; // double-buffer
};
```



This lets you **submit** frame data safely without locking.  
The render thread will always consume the previous frame’s data while the engine thread is producing the next one.



### I want to run all the render logic on a separated thread.

By dedicating a thread to it, I will:

* Avoid stalling simulation/game logic (Engine).

* Keep GPU submission timings consistent.

* Allow background frame preparation.

Essentially, the **render thread** is responsible for:

* Collecting render commands (submitted by the main or engine thread).

* Building command buffers.

* Submitting them to GPU queues.



### Example of a Render System

```cpp
class Renderer {
public:
    void StartRenderThread();
    void StopRenderThread();
    void SubmitFrame(const FrameData& frame);

private:
    void RenderLoop();
    std::thread renderThread;
    std::atomic<bool> running{false};
    RenderQueue queue;
};

void Renderer::StartRenderThread() {
    running = true;
    renderThread = std::thread(&Renderer::RenderLoop, this);
}

void Renderer::StopRenderThread() {
    running = false;
    if (renderThread.joinable()) renderThread.join();
}

void Renderer::SubmitFrame(const FrameData& frame) {
    queue.Push(frame);
}

void Renderer::RenderLoop() {
    while (running) {
        FrameData frame;
        if (queue.Pop(frame)) {
            // Record command buffers
            // Submit to GPU
            // Present
        } else {
            std::this_thread::yield(); // or sleep a bit
        }
    }
}
```

| Layer        | Threaded Behavior                                                                         |
| ------------ | ----------------------------------------------------------------------------------------- |
| **Core**     | Provides thread primitives, atomics wrappers, and job system.                             |
| **Engine**   | Runs simulation & prepares `FrameData`. Calls `renderer.SubmitFrame()`.                   |
| **Renderer** | Runs independently on render thread. Uses data snapshots only (no shared mutable access). |
| **App**      | Manages lifetime & synchronization (starts/stops threads cleanly).                        |



The Core Layer will need to include 

| Subsystem                       | Description                                                              |
| ------------------------------- | ------------------------------------------------------------------------ |
| **Threading Primitives**        | Abstractions over `std::thread`, `std::mutex`, `std::condition_variable` |
| **Atomic Wrappers**             | Wrappers over `std::atomic<T>` for consistency and custom semantics      |
| **Job System / Task Scheduler** | Thread pool that runs jobs (used by physics, rendering, simulation)      |
| **Logger / Profiler**           | Core utilities used by all layers                                        |
| **Memory / Allocators**         | Optional custom allocators for high-performance pools                    |


