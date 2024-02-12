#include "perlin_noise.h"
#include <math.h>
#include <stdlib.h>
#include <time.h> // Include for seeding the random number generator
#include <stdio.h>

static int p[512];

static int repeat = 256;

static int inc(int num) {
    num++;
    if (repeat > 0) num %= repeat;
    return num;
}

static float fade(float t) { 
    return t * t * t * (t * (t * 6 - 15) + 10); 
}

static float lerp(float t, float a, float b) { 
    return a + t * (b - a); 
}

static float grad(int hash, float x, float y, float z) {
    int h = hash & 15;
    float u = h < 8 ? x : y,
          v = h < 4 ? y : h == 12 || h == 14 ? x : z;
    return ((h&1) == 0 ? u : -u) + ((h&2) == 0 ? v : -v);
}

float noiseMin = 0;
float noiseMax = 1;

float perlin(float x, float y, float z) {
    if(repeat > 0) {
        x = fmod(fabs(x), repeat);
        y = fmod(fabs(y), repeat);
        z = fmod(fabs(z), repeat);
    }

    int xi = (int)x & 255;
    int yi = (int)y & 255;
    int zi = (int)z & 255;
    float xf = x - (int)x;
    float yf = y - (int)y;
    float zf = z - (int)z;
    float u = fade(xf);
    float v = fade(yf);
    float w = fade(zf);

    int aaa, aba, aab, abb, baa, bba, bab, bbb;
    aaa = p[p[p[    xi ]+    yi ]+    zi ];
    aba = p[p[p[    xi ]+inc(yi)]+    zi ];
    aab = p[p[p[    xi ]+    yi ]+inc(zi)];
    abb = p[p[p[    xi ]+inc(yi)]+inc(zi)];
    baa = p[p[p[inc(xi)]+    yi ]+    zi ];
    bba = p[p[p[inc(xi)]+inc(yi)]+    zi ];
    bab = p[p[p[inc(xi)]+    yi ]+inc(zi)];
    bbb = p[p[p[inc(xi)]+inc(yi)]+inc(zi)];

    float x1, x2, y1, y2;
    x1 = lerp(grad (aaa, xf  , yf  , zf),           
              grad (baa, xf-1, yf  , zf),             
              u);                                     
    x2 = lerp(grad (aba, xf  , yf-1, zf),           
              grad (bba, xf-1, yf-1, zf),             
              u);                                     
    y1 = lerp(x1, x2, v);                             

    x1 = lerp(grad (aab, xf  , yf  , zf-1),           
              grad (bab, xf-1, yf  , zf-1),             
              u);                                     
    x2 = lerp(grad (abb, xf  , yf-1, zf-1),
              grad (bbb, xf-1, yf-1, zf-1),
              u);
    y2 = lerp(x1, x2, v);

    float finalValue = lerp(y1, y2, w);
    if (finalValue < noiseMin) noiseMin = finalValue;
    if (finalValue > noiseMax) noiseMax = finalValue;
    float normalizedValue = (finalValue - noiseMin) / (noiseMax - noiseMin); // now it's between 0 and 1
    return normalizedValue;
    
}

void initPerlin() {
    int i, j, temp;

    // Seed the random number generator with the current time
    srand((unsigned int)time(NULL));

    // Initialize array with values from 0 to 255
    for(i = 0; i < 256; i++) {
        p[i] = i;
    }

    // Shuffle array using the Fisher-Yates shuffle algorithm
    for(i = 255; i > 0; i--) {
        j = rand() % (i + 1);

        temp = p[i];
        p[i] = p[j];
        p[j] = temp;
    }

    // Duplicate the permutation to avoid overflow
    for(i = 0; i < 256; i++) {
        p[256 + i] = p[i];
    }
}

float perlin2D(float x, float y) {
    return perlin(x, y, 0.0);
}
