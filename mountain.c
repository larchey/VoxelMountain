#include <SDL2/SDL.h>
#include <GL/gl.h>
#include <GL/glu.h>
#include <math.h>
#include "perlin_noise.h"

#define WINDOW_WIDTH 600
#define WINDOW_HEIGHT 600

int offsetX = 0, offsetY = 0; // Global variables to track the offset
int maxHeight = 10; // Maximum height of the cubes
void drawCube(float x, float y, float z, int k, int maxHeight) {
    float ratio = (float)k / maxHeight;
    float colorIntensity;

    if (ratio <= 0.25) {
        // Blue (water) - dark to light
        colorIntensity = ratio / 0.25;
        glColor3f(0.0f, 0.0f, colorIntensity);
    } else if (ratio <= 0.5) {
        // Green (grass) - dark to light
        colorIntensity = (ratio - 0.25) / 0.25;
        glColor3f(0.0f, colorIntensity, 0.0f);
    } else if (ratio <= 0.75) {
        // Brown (dirt) - dark to light
        colorIntensity = (ratio - 0.5) / 0.25;
        glColor3f(colorIntensity * 0.6, colorIntensity * 0.4, colorIntensity * 0.2);
    } else {
        // White (snow) - dark to light
        colorIntensity = (ratio - 0.75) / 0.25;
        glColor3f(colorIntensity, colorIntensity, colorIntensity);
    }

    glBegin(GL_QUADS);

    // Top face
    glVertex3f(x - 0.5f, y + 0.5f, z - 0.5f);
    glVertex3f(x + 0.5f, y + 0.5f, z - 0.5f);
    glVertex3f(x + 0.5f, y + 0.5f, z + 0.5f);
    glVertex3f(x - 0.5f, y + 0.5f, z + 0.5f);

    // Right face
    glVertex3f(x + 0.5f, y - 0.5f, z - 0.5f);
    glVertex3f(x + 0.5f, y + 0.5f, z - 0.5f);
    glVertex3f(x + 0.5f, y + 0.5f, z + 0.5f);
    glVertex3f(x + 0.5f, y - 0.5f, z + 0.5f);

    // Back face
    glVertex3f(x - 0.5f, y - 0.5f, z + 0.5f);
    glVertex3f(x + 0.5f, y - 0.5f, z + 0.5f);
    glVertex3f(x + 0.5f, y + 0.5f, z + 0.5f);
    glVertex3f(x - 0.5f, y + 0.5f, z + 0.5f);
    glEnd();
}

void drawGrid() {
    float noiseScale = 0.095; // Adjust for more or less frequent "hills"

    for (int i = 0; i < 20; ++i) {
        for (int j = 0; j < 20; ++j) {
            float noiseValue = perlin2D((i + offsetX) * noiseScale, (j + offsetY) * noiseScale);
            int height = (int)floor(noiseValue * maxHeight);
            for (int k = 0; k <= height; ++k) {
                drawCube(i - 10, k, j - 10, k, maxHeight);
            }
        }
    }
}
void setupIsometricView(int width, int height) {
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(45.0, (double)width / (double)height, 0.1, 100.0);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    gluLookAt(20, 30, 30, 0, 0, 0, 0, 1, 0);
    glEnable(GL_POLYGON_SMOOTH);
    glHint(GL_POLYGON_SMOOTH_HINT, GL_NICEST);
}

int main(int argc, char *argv[]) {
    SDL_Init(SDL_INIT_VIDEO);
    SDL_Window *window = SDL_CreateWindow("Isometric Grid",
                                          SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
                                          WINDOW_WIDTH, WINDOW_HEIGHT,
                                          SDL_WINDOW_OPENGL);
    SDL_GLContext glContext = SDL_GL_CreateContext(window);

    initPerlin(); // Initialize Perlin noise

    setupIsometricView(WINDOW_WIDTH, WINDOW_HEIGHT);

    while (1) {
        SDL_Event event;
        int quit = 0; // Flag to indicate quitting

        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT) {
                quit = 1; // Set quit flag
                break;
            } else if (event.type == SDL_KEYDOWN) {
                switch (event.key.keysym.sym) {
                    case SDLK_w: offsetY--; break;
                    case SDLK_s: offsetY++; break;
                    case SDLK_a: offsetX--; break;
                    case SDLK_d: offsetX++; break;
                    case SDLK_ESCAPE: // Handle Escape key
                        quit = 1; // Set quit flag
                        break;
                }
            }
        }

        if (quit) break; // Break out of the main loop if quit flag is set

        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        glEnable(GL_DEPTH_TEST);

        drawGrid();

        SDL_GL_SwapWindow(window);
    }

    SDL_GL_DeleteContext(glContext);
    SDL_DestroyWindow(window);
    SDL_Quit();

    return 0;
}