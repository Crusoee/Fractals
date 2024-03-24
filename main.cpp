#include "raylib.h"
#include "math.h"
#include <math.h>
#include <stdarg.h>
#include <stdio.h>
#include <iostream>
#include <thread>
#include <vector>
#include <mutex>
#include <atomic>

// for web
// Color ColorGradient(int iter, int maxIter) {
//     // Define your color gradient here
//     // You can change this function to create different color gradients
//     float t = (float)iter / maxIter;

//     unsigned char red = (unsigned char)(255 * t);
//     unsigned char green = (unsigned char)(255 * t);
//     unsigned char blue = (unsigned char)(255 / t); // Note the change here

//     return (Color){red, green, blue, 255};
// }

Color ColorGradient(int iter, int maxIter) {
    // Define your color gradient here
    // You can change this function to create different color gradients
    float t = (float)iter/maxIter;

    return (Color){ (unsigned char)255*t, (unsigned char)255*t, (unsigned char)255*t, 255 };

}

int main(void) {

    //2560
    const int screenWidth = 800;
    const int screenHeight = 800;
    float zoom = 4;
    float offsetX = 0.0;
    float offsetY = 0.0;
    float cReal = 0.0; // Real part of the complex constant c
    float cImag = 0.0; // Imaginary part of the complex constant c

    int MAX_ITERATIONS = 70;

    InitWindow(screenWidth, screenHeight, "Julia Set");

    // ToggleFullscreen();

    SetTargetFPS(60);

    while (!WindowShouldClose()) {
        // Zoom in/out based on mouse wheel inpu  
        int wheel = GetMouseWheelMove();
        if (wheel > 0) {
            zoom *= 0.9; // Zoom in
        } else if (wheel < 0) {
            zoom /= 0.9; // Zoom out
        }

        if (IsKeyDown(KEY_LEFT_SHIFT)) {
            if (IsKeyDown(KEY_A)) offsetX -= 0.01f * zoom;
            if (IsKeyDown(KEY_D)) offsetX += 0.01f * zoom;
            if (IsKeyDown(KEY_W)) offsetY -= 0.01f * zoom;
            if (IsKeyDown(KEY_S)) offsetY += 0.01f * zoom;
        }
        else{
            if (IsKeyDown(KEY_A)) offsetX -= 0.13f * zoom;
            if (IsKeyDown(KEY_D)) offsetX += 0.13f * zoom;
            if (IsKeyDown(KEY_W)) offsetY -= 0.13f * zoom;
            if (IsKeyDown(KEY_S)) offsetY += 0.13f * zoom;
        }
        if (IsKeyDown(KEY_R)) MAX_ITERATIONS += 10;
        if (IsKeyDown(KEY_F) && MAX_ITERATIONS > 0) MAX_ITERATIONS -= 10;

        if (IsMouseButtonDown(MOUSE_BUTTON_LEFT)) {
            Vector2 center {screenWidth / 2, screenHeight / 2};

            cReal = (GetMousePosition().x - center.x) * .003;
            cImag = (GetMousePosition().y - center.y) * .003;
        }
        // if (IsKeyPressed(KEY_Z)) {
        //     // Save the texture as a PNG file
        //     ExportImage(LoadImageFromScreen(), "fractal.png");
        // }

        BeginDrawing();

        ClearBackground(BLACK);

        // Iterate over each pixel of the screen
        for (int y = 0; y < screenHeight; y++) {
            for (int x = 0; x < screenWidth; x++) {
                // Convert pixel coordinates to complex number coordinates
                float real = (x - screenWidth / 2.0) * zoom / screenWidth + offsetX;
                float imag = (y - screenHeight / 2.0) * zoom / screenHeight + offsetY;
                float complexPart = imag;
                float realPart = real;

                // Iterate the equation for a fixed number of times or until magnitude exceeds a threshold
                int iter;
                for (iter = 0; iter < MAX_ITERATIONS; iter++) {
                    // Julia Fractal
                    float temp = realPart * realPart - complexPart * complexPart + cReal;
                    complexPart = 2 * realPart * complexPart + cImag;
                    realPart = temp;

                    // Check if the magnitude exceeds a threshold
                    if (realPart * realPart + complexPart * complexPart > 4) break;
                }

                // Color pixel based on number of iterations
                Color color = (iter == MAX_ITERATIONS) ? BLACK : ColorGradient(iter, MAX_ITERATIONS);
                DrawPixel(x, y, color);
            }
        }

        DrawFPS(100,100);

        EndDrawing();

        std::cout << "zoom: " << zoom << " xoffset: " << offsetX << " yoffset: " << offsetY << " cReal: " << cReal << " cImag: " << cImag << " Max It: " << MAX_ITERATIONS << std::endl;
    }

    CloseWindow();
    return 0;
}