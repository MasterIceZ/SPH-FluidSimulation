# SPH Fluid Simulation

This repository contains source code for a simple lagrangian fluid simulation using Smoothed Particle Hydrodynamics (SPH) method. This work is also a final project for the course **Realtime Computer Graphics and Physics Simulation** at [Chulalongkorn University](https://chula.ac.th).

By implementing the SPH method, we can simulate fluid dynamics in a more realistic way. The simulation is based on the paper:

- [Particle-Based Fluid Simulation for Interactive Applications](https://matthias-research.github.io/pages/publications/sca03.pdf)

## Framework

- C++
- OpenGL
- GLFW

## Installation

We are using GLAD to load OpenGL functions. You can install it by using generator here: [GLAD](https://glad.dav1d.de/#profile=compatibility&language=c&specification=gl&loader=on&api=gl%3D4.6)

Then, copy the generated files to `include/glad/glad.h`, `src/glad.c` and `include/KHR/khrplatform.h`.

## Running the Code

Build and Run using Makefile:

```
make MAIN_FILE=src/main.cpp
```

## Contributors

- [Dej Wongwirathorn](https://github.com/kyrov)
- [Borworntat Dendumrongkul](https://github.com/mastericez)
