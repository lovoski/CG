# CG (Computer Graphics)

## Introduction

This is a collection of my computer graphics related implementations. Including physics simulation (PhySim), software renderer (Renderer) and some math (Math).

## Compile

This project is built with MinGW under window. All the dependencies have been contained as submodule in the `external` folder, so add the `--recursive` flag when cloning the repository.

The dependencies are listed as follows:

1. raylib: simple visualization
2. eigen3: vector math
3. raygui: gui components
4. rlimgui(imgui): same as raygui

It is recommended to download MinGW from [w64devkit](https://github.com/skeeto/w64devkit). To compile the project, [cmake](https://cmake.org/) is also required.

```cmd
git clone --recursive https://github.com/lovoski/CG.git
cd CG
mkdir build
cd build
cmake ..
make
```

## Implement Details

### Physics Simulation

The learning resources includes [Fluid Simulation for Computer Graphics](https://www.cs.ubc.ca/~rbridson/fluidsimulation/) by Robert Bridson.

The first part is fluid simulations, includes eulerian method, PIC, APIC, FLIP.

The second part is more complex, includes MPM.

### Rasterize Renderer

The learning resource includes [tinyrenderer](https://github.com/ssloy/tinyrenderer) by ssloy, [games101](https://sites.cs.ucsb.edu/~lingqi/teaching/games101.html) by [Lingqi Yan](https://sites.cs.ucsb.edu/~lingqi/index.html).

### Path Tracer

The learning resource includes [RayTracingInOneWeekend](https://raytracing.github.io/books/RayTracingInOneWeekend.html) by Shirley et al.
