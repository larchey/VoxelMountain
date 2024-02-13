# Voxel terrain with opengl and Simplex or Perlin Noise

This project demonstrates the creation of an isometric grid using OpenGL and SDL2, with terrain height variation simulated using Simplex ~~Perlin~~  noise. The grid consists of cubes whose heights vary according to the Simplex ~~Perlin~~  noise value at their grid position, creating a simple, procedurally generated landscape.

## Demo

Here is a demonstration of the project:

![Demo Gif](./demo/demo.gif)

## Prerequisites

Before you can build and run this project, you need to have the following installed:
- SDL2
- OpenGL
- A C compiler (e.g., GCC)

## Building the Project

To build this project, follow these steps:

1. **Install Dependencies**

   Make sure you have SDL2 and OpenGL installed on your system. The installation process varies depending on your operating system.

   - On Ubuntu/Debian-based systems, you can install SDL2 and OpenGL using the following command:
     ```
     sudo apt-get install libsdl2-dev libglu1-mesa-dev freeglut3-dev mesa-common-dev
     ```

   - On macOS, you can use Homebrew to install SDL2:
     ```
     brew install sdl2
     ```

   OpenGL should already be available on macOS.

   - On Windows, you can download the SDL2 development libraries from the SDL website and place them in your project directory. For OpenGL, most Windows systems should have it pre-installed.

2. **Compile the Code**

   Navigate to the directory containing your project files in the terminal, and compile the code using the following command:

   ```
   gcc -g terrain.c noise/simplex/simplex_noise.c -o voxel_terrain -lGL -lGLU -lSDL2 -lGLEW -lm
   ```

   This command compiles the `terrain.c` and `simplex_noise.c` files, linking against the SDL2, OpenGL, and GLU libraries, and produces an executable named `voxel_terrain`.

3. **Run the Program**

   After compiling, you can run the program by executing the following command in the terminal:

   ```
   ./voxel_terrain
   ```

   This will open a window displaying the isometric grid with varying cube heights based on Simplex ~~Perlin~~  noise.

## Notes

- The `drawCube` function is used to draw individual cubes at specified positions with varying heights.
- The `drawGrid` function generates the grid by calculating the height of each cube using Simplex ~~Perlin~~  noise.
- The `setupIsometricView` function sets up the isometric view using OpenGL.

Enjoy exploring the procedurally generated landscape!