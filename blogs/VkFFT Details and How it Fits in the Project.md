# VkFFT Details and how it fits in the Project

### Main Overview

- VkFFT is a Fast Fourier Transform Library that runs FFTs on the GPU using compute shaders and highly optimized algorithms.

- It works on NVIDIA, AMD, INTEL, APPLE, and other GPUs that have Vulkan support

- Header-only API that integrates the existing Vulkan command buffers 

- It doesn't manage the memory allocations
  
  

## Reason why we chose this library

    It runs on the GPU so allows us to get data extremely quickly for large transforms. It also works on the GPU, so less CPU-GPU transfer latency time. 





## How it fits in the Layers Architecture of the Project?

The Renderer Layer will call the VkFFT functions since vkFFT needs Vulkan command buffers. 

The Engine Layer will produce the pure data, the initial spectrum and parameters.

The Renderer uses that data,, from the Engine Layer, and dispactches vkFFT inside Vulkan command buffers.





## How does vkFFT work?

We have the vkFFT.h file in the Renderer subfolder. We have to create a vkFFTConfiguration struct with the FFT parameters. Then we add the FFT dispatches to the command buffers using vkfftAppend. Then we just submit the command buffer as normal.

In the **Renderer**:

1. Since the vkFFT API works directly with Buffers and Images we need to **Allocate Vulkan buffers/images** for FFT input and output. The Ocean FFT data must persist between frames and since the we call Vulkan functions that operate on the output data, we need to save the data in renderer-managed GPU memory.

2. **Fill FFT configuration object**, more percisely vkFFT needs Trnasform size, percision either FP16 or FP32, storage pointers and layout, either in-place or out-of-place.

3. **Initialize VkFFT plan**, typically we are going to only create one plan and reuse it every frame, since plan creation is expensive and not frame-dependent.

4. In your render pass recording, _before drawing_:
   
   * Bind the appropriate compute pipeline or use VkFFT API to append into the command buffer.
   
   * Provide memory barrier(s) afterwards to make FFT output available to subsequent shaders.

5. Continue with graphics or compute that consumes FFT result.


