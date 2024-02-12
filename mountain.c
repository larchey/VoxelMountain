#include <SDL2/SDL.h>
#include <GL/gl.h>
#include <GL/glu.h>
#include <math.h>
#include "perlin_noise.h"

#define WINDOW_WIDTH 600
#define WINDOW_HEIGHT 600

int offsetX = 0, offsetY = 0; // Global variables to track the offset
int offsetScale = 10;
void drawCube(float x, float y, float z) {
    glBegin(GL_QUADS);

    // Top face
    glColor3f(1.0f, 0.0f, 0.0f); // Red
    glVertex3f(x - 0.5f, y + 0.5f, z - 0.5f);
    glVertex3f(x + 0.5f, y + 0.5f, z - 0.5f);
    glVertex3f(x + 0.5f, y + 0.5f, z + 0.5f);
    glVertex3f(x - 0.5f, y + 0.5f, z + 0.5f);

    // Bottom face
    glColor3f(0.0f, 1.0f, 0.0f); // Green
    glVertex3f(x - 0.5f, y - 0.5f, z - 0.5f);
    glVertex3f(x + 0.5f, y - 0.5f, z - 0.5f);
    glVertex3f(x + 0.5f, y - 0.5f, z + 0.5f);
    glVertex3f(x - 0.5f, y - 0.5f, z + 0.5f);

    // Front face
    glColor3f(0.0f, 0.0f, 1.0f); // Blue
    glVertex3f(x - 0.5f, y - 0.5f, z + 0.5f);
    glVertex3f(x + 0.5f, y - 0.5f, z + 0.5f);
    glVertex3f(x + 0.5f, y + 0.5f, z + 0.5f);
    glVertex3f(x - 0.5f, y + 0.5f, z + 0.5f);

    // Back face
    glColor3f(1.0f, 1.0f, 0.0f); // Yellow
    glVertex3f(x - 0.5f, y - 0.5f, z - 0.5f);
    glVertex3f(x + 0.5f, y - 0.5f, z - 0.5f);
    glVertex3f(x + 0.5f, y + 0.5f, z - 0.5f);
    glVertex3f(x - 0.5f, y + 0.5f, z - 0.5f);

    // Left face
    glColor3f(1.0f, 0.0f, 1.0f); // Magenta
    glVertex3f(x - 0.5f, y - 0.5f, z - 0.5f);
    glVertex3f(x - 0.5f, y - 0.5f, z + 0.5f);
    glVertex3f(x - 0.5f, y + 0.5f, z + 0.5f);
    glVertex3f(x - 0.5f, y + 0.5f, z - 0.5f);

    // Right face
    glColor3f(0.0f, 1.0f, 1.0f); // Cyan
    glVertex3f(x + 0.5f, y - 0.5f, z - 0.5f);
    glVertex3f(x + 0.5f, y - 0.5f, z + 0.5f);
    glVertex3f(x + 0.5f, y + 0.5f, z + 0.5f);
    glVertex3f(x + 0.5f, y + 0.5f, z - 0.5f);

    glEnd();

}

void drawGrid() {
    float noiseScale = 0.1; // Adjust for more or less frequent "hills"
    int maxHeight = 5; // Maximum height of the cubes

    for (int i = 0; i < 20; ++i) {
        for (int j = 0; j < 20; ++j) {
            float noiseValue = perlin((i + offsetX) * noiseScale, (j + offsetY) * noiseScale, 0.0);
            int height = (int)floor(noiseValue * maxHeight);
            for (int k = 0; k <= height; ++k) {
                drawCube(i - 10, k, j - 10);
            }
        }
    }
}

void setupIsometricView(int width, int height) {
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(30.0, (double)width / (double)height, 0.1, 100.0);
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
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT) {
                break;
            } else if (event.type == SDL_KEYDOWN) {
                switch (event.key.keysym.sym) {
                    case SDLK_w: offsetY -= offsetScale; break;
                    case SDLK_s: offsetY += offsetScale; break;
                    case SDLK_a: offsetX -= offsetScale; break;
                    case SDLK_d: offsetX += offsetScale; break;
                }
            }
        }

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