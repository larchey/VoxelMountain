#include <SDL2/SDL.h>
#include <GL/glew.h>
#include <GL/gl.h>
#include <GL/glu.h>
#include <math.h>
// #include "noise/perlin/perlin_noise.h" 
#include "noise/simplex/simplex_noise.h"

#define WINDOW_WIDTH 600
#define WINDOW_HEIGHT 600

int offsetX = 0, offsetY = 0; // Global variables to track the offset
int maxHeight = 10; // Maximum height of the cubes

void setCubeColor(int k, int maxHeight) {
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
}

GLuint createCubeVBO() {
    // Define the vertices for a cube
    GLfloat cubeVertices[] = {
        // Positions
        // Front face
        -0.5f, -0.5f,  0.5f,  // Bottom-left
         0.5f, -0.5f,  0.5f,  // Bottom-right
         0.5f,  0.5f,  0.5f,  // Top-right
        -0.5f,  0.5f,  0.5f,  // Top-left

        // Right face
         0.5f, -0.5f,  0.5f,  // Bottom-left
         0.5f, -0.5f, -0.5f,  // Bottom-right
         0.5f,  0.5f, -0.5f,  // Top-right
         0.5f,  0.5f,  0.5f,  // Top-left

        // Top face
        -0.5f,  0.5f,  0.5f,  // Bottom-left
         0.5f,  0.5f,  0.5f,  // Bottom-right
         0.5f,  0.5f, -0.5f,  // Top-right
        -0.5f,  0.5f, -0.5f,  // Top-left
    };

    // Create a VBO for the cube geometry
    GLuint cubeVBO;
    glGenBuffers(1, &cubeVBO);
    glBindBuffer(GL_ARRAY_BUFFER, cubeVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(cubeVertices), cubeVertices, GL_STATIC_DRAW);
    
    // Specify the layout of the vertex data
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (void*)0);
    glEnableVertexAttribArray(0);


    GLenum err;
    while ((err = glGetError()) != GL_NO_ERROR) {
        printf("OpenGL error: %d\n", err);
    }
    return cubeVBO;
}

void drawGrid(GLuint cubeVBO) {
    float noiseScale = 0.05;

    // Bind the cube VBO
    glBindBuffer(GL_ARRAY_BUFFER, cubeVBO);

    for (int i = 0; i < 20; ++i) {
        for (int j = 0; j < 20; ++j) {
            float noiseValue = simplex2D((i + offsetX) * noiseScale, (j + offsetY) * noiseScale);
            int height = (int)floor(noiseValue * maxHeight);
            for (int k = 0; k <= height; ++k) {
                // Set the cube color
                setCubeColor(k, maxHeight);

                // Save the current matrix
                glPushMatrix();

                // Translate the cube to its position in the grid
                glTranslatef(i, k, j);

                // Draw the cube
                glDrawArrays(GL_QUADS, 0, 24);

                // Restore the previous matrix
                glPopMatrix();
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
    gluLookAt(30, 30, 40, 2, -8, 0, 0, 1, 0); 
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
    glewExperimental = GL_TRUE;
    if (glewInit() != GLEW_OK) {
        printf("Failed to initialize GLEW\n");
        return -1;
    }

    printf("OpenGL version: %s\n", glGetString(GL_VERSION));
    setupIsometricView(WINDOW_WIDTH, WINDOW_HEIGHT);

    // Create the cube VBO
    GLuint cubeVBO = createCubeVBO();

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

        // Pass the cube VBO to the drawGrid function
        drawGrid(cubeVBO);

        SDL_GL_SwapWindow(window);
    }

    SDL_GL_DeleteContext(glContext);
    SDL_DestroyWindow(window);
    SDL_Quit();

    return 0;
}