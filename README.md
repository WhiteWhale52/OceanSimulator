# Ocean Simulator
## Overview
This project implements a high-performance, visually convincing fluid simulation framework for games and real-time applications. It combines principles from Eulerian and kinetic solvers with GPU optimization and FFT-based ocean wave synthesis to achieve physically inspired yet efficient results.

## Key Features

### Stable Real-Time Fluid Dynamics
Based on Jos Stam’s “Real-Time Fluid Dynamics for Games”, the solver provides visually stable, unconditionally stable Navier–Stokes integration suitable for smoke, water, and gas effects.

### Ocean Surface Simulation
Implements Jerry Tessendorf’s “Simulating Ocean Water” model using FFT-based spectral synthesis for realistic deep-water waves, reflections, refraction, and light scattering.

### GPU-Accelerated Kinetic Solver
Utilizes concepts from “GPU Optimization for High-Quality Kinetic Fluid Simulation”, leveraging the ACM-MRT lattice Boltzmann formulation for parallel, high-quality turbulent flow simulation with complex boundary handling.

### FFT on the GPU
Integrates efficient GPU-based FFT techniques from “The FFT on a GPU” to accelerate spectrum synthesis and filtering operations directly on the graphics hardware.

### Height-Field Simulation for Sand-Water Mixtures (Very Far in the Future)
Extends “Real-Time Height-Field Simulation of Sand and Water Mixtures” to handle two-phase shallow-water and granular flow dynamics in real time, combining elastoplastic sand behavior with water flow and diffusion.

### Optimized GPU Pipeline
* Custom memory layout and multi-kernel strategy for maximum coalesced access.
* Load-balanced immersed boundary implementation.
* Multi-GPU support for large-scale simulations.

## Applications
* Real-time game engines (Unity/Unreal)
* Interactive terrain and ocean scenes
* GPU-based research in fluid and kinetic simulation


## Acknowledgements
This project builds on the foundational works of:

David Algis (2025) - Arc Blanc: a real time ocean simulation framework

Haozhe Su & Xifeng Gao (2023) - Real-time Height-field Simulation of Sand and Water Mixtures

Jerry Tessendorf (2001–2004) – Simulating Ocean Water

Christopher J. Horvath - Empirical Directional Wave Spectra for Computer Graphics

Yixin Chen et al. (2022) – GPU Optimization for High-Quality Kinetic Fluid Simulation

Kenneth Moreland & Edward Angel (2003) – The FFT on a GPU

Haozhe Su et al. (2023) – Real-Time Height-Field Simulation of Sand and Water Mixtures
