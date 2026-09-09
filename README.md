# 🔍 Overview

This project implements a real-time, FFT-driven water simulation system capable of rendering large ocean surfaces with realistic wave motion, choppiness, and wind interaction — all computed entirely on the GPU.

Instead of solving full 3D Navier–Stokes equations, the simulation represents the ocean surface as a heightfield derived from a wave energy spectrum in the frequency domain. The surface is updated each frame using Fast Fourier Transforms (FFTs), allowing thousands of interacting waves to evolve seamlessly and in real time.


[![Builds on Linux (Ubuntu)](https://github.com/WhiteWhale52/OceanSimulator/actions/workflows/Linux-Build.yml/badge.svg?branch=main)](https://github.com/WhiteWhale52/OceanSimulator/actions/workflows/Linux-Build.yml)    
[![Building on Windows with Caching](https://github.com/WhiteWhale52/OceanSimulator/actions/workflows/Windows-Builds-Optimized.yml/badge.svg?branch=main)](https://github.com/WhiteWhale52/OceanSimulator/actions/workflows/Windows-Builds-Optimized.yml)  
[![MacOS Build Optimized](https://github.com/WhiteWhale52/OceanSimulator/actions/workflows/MacOS-Build-Optimized.yml/badge.svg)](https://github.com/WhiteWhale52/OceanSimulator/actions/workflows/MacOS-Build-Optimized.yml)
#  Motivations

### Why does this project matter to me?

* This is a real test of my skills. I have spent 2025 summer honing my research skills and my project development skills. This project is not only my first attempt at using the research that I read in a project but it is also my most-ambious project idea. 

* The project wasn't chosen to be the most complex, but it is something that interests me, the fluid mechanics and GI.

### Who will I be proud to show it to?

* Anyone really. Just anyone, I want them to see my dedication and resilience when moving through new terratories. Hopefully, this project would be the first of many.

* I think I would be most proud of this project once it is complete. Since this is **MINE**, my work my time my design. No one can take that away from me.

# 🎯 Goals

<img width="1910" height="903" alt="image" src="https://github.com/user-attachments/assets/6929769a-bdcc-4147-b3c5-30c7f7a3eead" />

* ### Real-time performance with physically grounded visuals

* ### Scalable to large ocean scenes using tile-based rendering

* ### GPU-only simulation for integration into real-time engines

* ### Extensible for coupling with particles or foam generation

# ⚙️ Key Features

* GPU-accelerated FFTs for high-performance spectral updates

* Heightfield-based ocean rendering with infinite tiling

* Directional wind-based wave generation

* Choppy wave displacements for natural asymmetry

* Energy-conserving amplitude correction

* Stable time integration for visually smooth, large-scale motion

* Implemented in C++ and compute shaders for maximum efficiency

# 🧠 Background and Influences

This project draws inspiration from several key works in computer graphics fluid simulation:

### Jerry Tessendorf — “Simulating Ocean Water”

The foundation of this project. Tessendorf introduced FFT-based spectral synthesis for ocean surfaces, where wave amplitudes and phases are generated from statistical ocean models. This allows complex, realistic surfaces to be computed efficiently.

### Jos Stam — “Stable Fluids for Games”

Stam’s method inspired the system’s stability and time integration strategies, ensuring smooth evolution without requiring tiny time steps — critical for real-time rendering.

### Horvath’s Improvements

Introduced directional spreading functions, choppy wave displacement, and spectral filtering. These make waves sharper, more wind-dependent, and visually richer by redistributing energy realistically across frequencies.

### Arc Blanc’s Refinements

Improved energy conservation and amplitude normalization in Tessendorf’s model. This ensures waves maintain consistent visual strength at different grid resolutions, avoiding the “flat” or “overblown” look common in uncorrected implementations.

---

_**Together, these techniques form a modern GPU ocean simulation pipeline — stable, scalable, and visually believable.**_

# 🧩 Technical Details

### Language: C++

### Rendering: Vulkan / GLSL

### Simulation Core: GPU-accelerated FFT (based on vkFFT)

### Data Representation: 2D complex frequency grids for displacement and choppiness

### Performance: Real-time simulation of 256×256 wave grids at 60+ FPS on mid-range GPUs

# 🌬️ How It Works

### Spectrum Generation:

The system initializes a Phillips or unified spectrum representing the ocean’s frequency energy based on wind direction, speed, and gravity constants.

### FFT Evolution:

Using a GPU-based FFT, the spectral data is advanced over time, evolving each wave’s amplitude and phase.

### Inverse FFT:

The frequency-domain data is transformed back into spatial heightfields and normal maps.

### Choppy Displacement:

Horvath’s horizontal displacement technique is applied to sharpen wave crests.

### Rendering:

The final heightfield is rendered using physically based shading, normal reconstruction, and seamless tiling.

# 📚 References

Jos Stam, “Real-Time Fluid Dynamics for Games”, GDC 2003

Jerry Tessendorf, “Simulating Ocean Water”, SIGGRAPH 2001

Horvath, “Spectral Wave Models for Film and Game Production”, Disney Animation

Arc Blanc, “Energy-Conserving Improvements to Tessendorf Spectra”

# 🧑‍💻 Author

Mohamed Tarek Mohamed 

Computer Science, University of Toronto 

Focused on real-time rendering, GPU systems, and simulation graphics.
