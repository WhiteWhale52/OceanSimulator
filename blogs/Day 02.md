### Day 2

* I read Empirical Directional Wave Spectra for Computer Graphics by Horvath
  
  * A thing I noticed is that Tessendorf is basically replacing direct Navier–Stokes simulation with a statistical spectral model. 
    
    

| Aspect                    | **Tessendorf (2001)**                                                                 | **Horvath (2015) – Empirical Directional Spectra**                                                                   |
| ------------------------- | ------------------------------------------------------------------------------------- | -------------------------------------------------------------------------------------------------------------------- |
| **Foundation**            | Based on Phillips spectrum (theoretical upper bound of wave energy).                  | Based on empirical spectra like Pierson–Moskowitz, JONSWAP, and TMA fitted to real ocean measurements.               |
| **Goal**                  | Efficient procedural ocean synthesis using FFT and dispersion relations.              | Same core idea, but more physically accurate and artist-friendly — no arbitrary tuning.                              |
| **Spectrum type**         | Uses a wave number spectrum ( $P(k) = A e^{-1/(kL)^2}/k^4 (\hat{k}\cdot\hat{w})^2 )$. | Uses measured energy spectra ( $S(\omega, \theta) = S(\omega)D(\omega, \theta)$ ) (TMA, JONSWAP, etc.).              |
| **Depth handling**        | Assumes deep water (no shallow correction).                                           | Includes depth attenuation using the Kitaigorodskii function Φ(ω, h).                                                |
| **Directional spreading** | Simple fixed ( $\cos^2(\theta)$ ) model.                                              | Uses empirical spreading functions (Mitsuyasu, Hasselmann, Donelan–Banner) and adds a “swell” parameter for control. |
| **Amplitude tuning**      | Manual, depends on wind and visual feedback.                                          | Automatic — calibrated to physical wind speed, fetch, and depth.                                                     |
| **Result**                | Good visuals, needs artistic tweaking.                                                | Physically accurate, consistent across conditions, less tuning needed.                                               |

Every empirical spectrum — Phillips, Pierson–Moskowitz, JONSWAP, TMA — is just a different model of how this energy is distributed based on real measurements or theory.

* JSONSWAP is an improvement from the Pierson-Moskowitz deep-water model, but it unfortunately assume deep water, where the depth of the water is larger than the wavelength of the waves.

* Kitaigorodskii et al. (1975) derived a **depth-dependent multiplier** Φ(ω,h) to scale down the deep-water energy spectrum appropriately.
  Formally:
  $Φ(ω,h)=\frac{(k(ω,h))^{−3}\frac{∂k(ω,h)​}{∂ω}​}{(k(ω,∞))^{−3}\frac{∂k(ω,∞)}{∂ω}}$
  That looks nasty, but it basically measures how the **wave number–frequency relationship** changes between deep and shallow water.

* The TMA Spectrum combines JSONSWAP with the depth-dependent multiplier.
  
  Therefore,
  
  When generating FFT-based ocean:
  
  1. Compute STMA​(ω) using the scene’s wind speed, fetch, and depth.
  
  2. Convert to wavenumber space S(k).
  
  3. Use that to determine each Fourier component’s amplitude 2S(k)Δkx​Δky​​.
  
  4. Evolve each component using dispersion ω(k) and inverse FFT to get h(x,t).
  
  Result:
  
  * In deep water (large h): behaves like JONSWAP.
  
  * In shallow water (small h): large waves are automatically damped, giving realistic shorelines.
