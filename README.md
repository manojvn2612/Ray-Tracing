# Ray Tracing in C++ with SDL

This project demonstrates a basic implementation of ray tracing using C++ and the Simple DirectMedia Layer (SDL) for rendering graphics. The code also uses standard mathematical functions for ray calculations.

## Table of Contents
- [Introduction](#introduction)
- [Requirements](#requirements)
- [Installation](#installation)
- [Usage](#usage)
- [How It Works](#how-it-works)
- [Future Improvements](#future-improvements)

## Introduction
Ray tracing is a technique for rendering images by simulating the way light interacts with objects. This project implements a simple version of ray tracing with SDL to visualize the graphics.

## Requirements
- C++17 or higher
- SDL3 library

## Installation
### Step 1: Clone the Repository
```bash
git clone https://github.com/manojvn2612/Ray-Tracing.git
cd Shadow_Casting
```

### Step 2: Install SDL

On Windows:
- Download the SDL development package from [https://libsdl.org/](https://libsdl.org/).

### Step 3: Compile the Program
use Visual Studio C++ version 17 or above
```bash
g++ -o ray_tracer Shadowcasting.cpp -lSDL3 -lm
```

## Usage
Run the compiled program:
```bash
./ray_tracer
```
This will launch a window displaying the ray-traced graphics.

## How It Works
1. **Graphics Rendering:** The SDL library is used to create a window and render graphics by filling pixels.
2. **Ray Tracing:** Rays are simulated to intersect with basic geometric objects, and the pixel colors are computed based on these interactions.
3. **Color Mapping:** Colors are calculated using basic math functions to visualize intersections.

## Code Overview
### Key Functions
- **Initialization:** Sets up SDL and creates a window.
- **Ray Calculation:** Mathematical functions to trace rays and detect object intersections.
- **Rendering:** Draws pixels on the window surface based on ray tracing results.

### Important Files
- `ray_tracing.cpp`: Main source file containing the implementation.

## Future Improvements
- Support for additional geometric objects (spheres, planes, and triangles).
- Implement shading models for realistic lighting.
- Add multi-threading for performance optimization.
- Support for texture mapping.
