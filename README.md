# SPH Fluid Simulation

This repository contains source code for a simple lagrangian fluid simulation using Smoothed Particle Hydrodynamics (SPH) method. This work is also a final project for the course **Realtime Computer Graphics and Physics Simulation** at [Chulalongkorn University](https://chula.ac.th).

By implementing the SPH method, we can simulate fluid dynamics in a more realistic way. The simulation is based on the paper:

- [Particle-Based Fluid Simulation for Interactive Applications](https://matthias-research.github.io/pages/publications/sca03.pdf)

## Framework

- OpenGL
- [GLFW](https://www.glfw.org/)
- [ImGui](https://github.com/ocornut/imgui/tree/master)

## Installation

We are using GLAD to load OpenGL functions. You can install it by using generator here: [GLAD](https://glad.dav1d.de/#profile=compatibility&language=c&specification=gl&loader=on&api=gl%3D4.6)

Then, copy the generated files to `include/glad/` and `include/KHR/`.

Installing ImGui

```sh
cd installation_scripts
./imgui.sh
```

## Running the Code

Build and Run using Makefile:

```
make MAIN_FILE=src/main.cpp
```

## References

- [Smoothed Particle Hydrodynamics Techniques for the Physics Based Simulation of Fluids and Solids](https://arxiv.org/abs/2009.06944)
- [Particle-Based Fluid Simulation for Interactive Applications](https://matthias-research.github.io/pages/publications/sca03.pdf)
- [Spatial Hashing](https://matthias-research.github.io/pages/tenMinutePhysics/11-hashing.pdf)

## Contributors

- [Borworntat Dendumrongkul](https://github.com/mastericez)
- [Dej Wongwirathorn](https://github.com/kyrov)
