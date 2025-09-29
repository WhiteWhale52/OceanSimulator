Date: 28/09/2025

### Theory

* Represent the heightfield using a complex spectrum $\tilde{h}=(\bold{k},t)$ 

* Synthesis heights using inverse FFT

* Use a statistically-based spectrum, for example Philips spectrum, to set initial amplitude and evolve them with the dispersion relation.

### Implementation details

1. I first need to choose an N x N grid where N is a power of two, since FFT is easiest on the GPU when the grid is a power of 2.

2. Build wavevector grid k and compute the ocean spectrum Ph​(k) (Philips or JONSWAP variants)

3. **Sample random phases**: generate complex Gaussian samples $\tilde{h_0}​(k)=​\frac{1}{\sqrt{2}}​(ξr​+iξ_i​)\sqrt{P_h​(k)}​$.

4. **Animate** using the dispersion relation ω(k) and form $\tilde{h}(k,t)=\tilde{h_0}​(k)e^{iωt}+ \tilde{h_0^*}​(−k)e^{−iωt}$.

5. **IFFT** to produce height samples h(x,t). Use a GPU FFT. I think I saw a GPU Gem article tackling running the FFT on the GPU efficiently

6. **Slope/normal**: compute slope via inverse FFT of $ik\tilde{h}(k,t)$ so normals are accurate even for small wavelengths. (preferred over finite differences).
   Practical note: compute height and slope FFTs in the frequency domain so I only need two IFFTs (one for height, two small ones for slope components or one combined complex output).

For implementing FFT on the GPU, I will be using VkFFT since the main project will be written in C++/Vulkan to help me improve my skills in both.

Implementation detail: do the 2D FFT as two passes of 1D FFT (rows then columns) with an efficient layout (pack complex numbers in float2). For Vulkan, device buffers/textures will be used.



### Shading + Rendering

* From height and slope compute normals (from FFT slope) for physically plausible Fresnel + reflection + environment sampling. Use a PBR reflection + fresnel term, optionally SSR for close-up. Use screen-space foam and foam generation by thresholding slope & crest height.

Local interactions
--------------------------------------------

* For interactive wakes, splashes, or a boat, overlay a **small local grid** and run a Stam stable-fluid solver (semi-Lagrangian advect + Gauss-Seidel diffusion + projection) restricted to a local patch. Stam’s solver is stable for arbitrary time steps and is compact to implement — very good to show you know the CFD foundations.

Performance & GPU optimization (makes your repo stand out)
----------------------------------------------------------

Adopt these techniques from the GPU paper you provided:

* **Use a GPU FFT library where possible** (VkFFT/cuFFT) instead of hand-rolled shader FFT unless you must. 

* **Memory layout & coalescing**: store vector data SoA or collected-SoA to maximize coalesced reads/writes on the GPU (especially for LBM/kinetic pieces). The GPU paper explains CSoA and grouping strategies.

* **Split heavy kernels**: if a compute kernel uses too many registers, split it into multiple launches to improve occupancy (advice from the ACM-MRT GPU paper).

* **Profiling & automated parameter search**: tune workgroup sizes and GPU buffer sizes; show a short benchmark table in your README (GPU memory use, ms per frame at different grid sizes) — reviewers love numbers.

“Wow” features that will make people stop scrolling
===================================================

* **Seamless tiling + camera LOD**: patch tiling with continuous LOD so horizon looks infinite. (Tessendorf patch approach.)

* **Foam & whitecap accumulation**: compute on GPU from crest detection and advect foam across the surface.

* **Ship wake + local coupling**: a small local Navier-Stokes or Stam patch coupled to the global FFT surface.

* **Interactive editing UI**  so you can show “author mode” screenshots. I think I will be using imGUI for this.

* **Perf comparison**: show the same scene running with CPU FFT vs GPU FFT vs VkFFT/cuFFT with numbers.
  
  

  **Document everything: cite the papers and include code comments explaining which equation is implemented where — that’s what proves you “know” the theory.**





### Vulkan initialization (short checklist)

------------------------------------------

* Create `VkInstance` (+ validation layers while developing).

* Pick `VkPhysicalDevice` with `VK_QUEUE_GRAPHICS_BIT` and `VK_QUEUE_COMPUTE_BIT` support (prefer one with discrete GPU).

* Create `VkDevice` and grab `VkQueue` handles for graphics and compute.

* Create 2 `VkCommandPool`s: one for graphics, one for compute (or share if same queue family).

* Create `VkDescriptorPool` and layout(s) for compute/graphics descriptors.  
  See the step-by-step Vulkan tutorial (the compute shader chapter is especially useful).



Minimal “what to code first” checklist (day-by-day mini-plan)
=============================================================

* **Day 1–2:** Vulkan app skeleton (window, instance, physical device, logical device, one graphics+compute queue). Get a triangle rendering working (follow vulkan-tutorial).

* **Day 2–4:** Implement buffer/image helpers, staging uploads, compile a simple compute shader and run a compute dispatch that writes to a storage buffer; copy to image and display that image on a quad. (This builds the plumbing you’ll use for FFT).

* **Day 4–6:** Implement CPU `h0` generator and upload to a `VkBuffer`. Implement the `spectral_update` compute shader and run it to produce `h(k,t)` in a buffer (no FFT yet). Visualize a trivial transform of that buffer to make sure indexing is correct.

* **Day 6–9:** Integrate VkFFT to perform inverse FFT from `h(k,t)` → real heights, copy result to image and render the displaced mesh. Use `VkFFT_TestSuite.cpp` as an example for config. 

* **Day 9–12:** Add choppy displacement (horizontal offsets), slope IFFTs for normals, and polishing shaders (Fresnel, environment map).

* **Day 12–15:** Add local Stam patch for wakes or a UI to tweak wind, patch size, choppiness, and record benchmark numbers.



## For resources to read and make sure to understand the task fully

Crest — production Unity ocean system (study architecture & shaders). [Ware Harmonics](https://github.com/wave-harmonic/crest)

VkFFT — efficient GPU FFT library (Vulkan/CUDA/OpenCL/Metal backends). [VkFFT](https://github.com/DTolm/VkFFT)

cuFFT (NVIDIA) — GPU FFT library and cuFFTDx for device fused FFTs. [NVIDIA Docs](https://docs.nvidia.com/cuda/cufft/)

FFTW / KissFFT — CPU FFT libraries (FFTW high performance, KissFFT simple). [FFTW](https://www.fftw.org/)

Realistic Ocean Simulation [UnityOcean](https://antoniospg.github.io/UnityOcean/OceanSimulation.html)


