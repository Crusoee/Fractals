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



//                                            -----------------------------Multithreaded TEST--------------------------------



// std::mutex allPixelsLock;

// int threadCounter =0;
// int cntr =0;

// std::atomic<int> semaphore{0};
// std::atomic<bool> finishClear{true};


// Color ColorGradient(int iter, int maxIter) {
//     // Define your color gradient here
//     // You can change this function to create different color gradients
//     float t = (float)iter/maxIter;

//     return (Color){ (unsigned char)255*t, (unsigned char)117*t, (unsigned char)24*t, 255 };

// }

// typedef struct Pixel {
//     int x;
//     int y;
//     Color color;
// }Pixel;

// typedef struct FractalArgs {
//     int screenWidth;
//     int screenHeight; 
//     float zoom; 
//     float offsetX; 
//     float offsetY; 
//     int MAX_ITERATIONS; 
//     float cReal; 
//     float cImag; 
//     // take out pixel values, no need to be inside the struct
//     std::vector<Pixel> allPixels;
// }FractalArgs;

// void CalculateFractal(FractalArgs* fractalArgs, int startX, int startY, int segX, int segY) {
//     for (int y = startY; y < segY; y++) {
//         for (int x = startX; x < segX; x++) {
//             // Convert pixel coordinates to complex number coordinates
//             float real = (x - fractalArgs->screenWidth / 2.0) * fractalArgs->zoom / fractalArgs->screenWidth + fractalArgs->offsetX;
//             float imag = (y - fractalArgs->screenHeight / 2.0) * fractalArgs->zoom / fractalArgs->screenHeight + fractalArgs->offsetY;
//             float complexPart = imag;
//             float realPart = real;

//             // Iterate the equation for a fixed number of times or until magnitude exceeds a threshold
//             int iter;
//             for (iter = 0; iter < fractalArgs->MAX_ITERATIONS; iter++) {
//                 // Julia Fractal
//                 float temp = realPart * realPart - complexPart * complexPart + fractalArgs->cReal;
//                 complexPart = 2 * realPart * complexPart + fractalArgs->cImag;
//                 realPart = temp;

//                 // Check if the magnitude exceeds a threshold
//                 if (realPart * realPart + complexPart * complexPart > 4) break;
//             }

//             // Color pixel based on number of iterations
//             Color color = (iter == fractalArgs->MAX_ITERATIONS) ? BLACK : ColorGradient(iter, fractalArgs->MAX_ITERATIONS);

//             Pixel pixel{x,y,color};

//             allPixelsLock.lock();
//                 fractalArgs->allPixels.push_back(pixel);
//                 // BeginTextureMode(target);
//                 //     DrawPixel(x,y,color);
//                 // EndTextureMode();
//             allPixelsLock.unlock();
//         }
//     }
// }

// int main(void) {
//     //2560


//     std::vector<std::thread> threads;

//     FractalArgs* fractalArgs = new FractalArgs();

    
//     // fractalArgs->startX = 0;
//     // fractalArgs->startY = 0;
//     // fractalArgs->segX = 800;
//     // fractalArgs->segY = 800;

//     // the threads don't wriite over any of this
//     fractalArgs->screenWidth = 800;
//     fractalArgs->screenHeight = 800;
//     fractalArgs->zoom = 2.0;
//     fractalArgs->offsetX = 0.0;
//     fractalArgs->offsetY = 0.0;
//     fractalArgs->cReal = 0.0; // Real part of the complex constant c
//     fractalArgs->cImag = 0.0; // Imaginary part of the complex constant c
//     fractalArgs->MAX_ITERATIONS = 70;


//     fractalArgs->allPixels.reserve(sizeof(Pixel) * pow(800,2));

//     InitWindow(fractalArgs->screenWidth, fractalArgs->screenHeight, "Julia Set");

//     // RenderTexture2D target = LoadRenderTexture(fractalArgs->screenWidth,fractalArgs->screenHeight);

//     // ToggleFullscreen();

//     SetTargetFPS(60);

//     while (!WindowShouldClose()) {
//         // Zoom in/out based on mouse wheel inpu  
//         int wheel = GetMouseWheelMove();
//         if (wheel > 0) {
//             fractalArgs->zoom *= 0.9; // Zoom in
//         } else if (wheel < 0) {
//             fractalArgs->zoom /= 0.9; // fractalArgs->Zoom out
//         }

//         if (IsKeyDown(KEY_LEFT_SHIFT)) {
//             if (IsKeyDown(KEY_A)) fractalArgs->offsetX -= 0.01f * fractalArgs->zoom;
//             if (IsKeyDown(KEY_D)) fractalArgs->offsetX += 0.01f * fractalArgs->zoom;
//             if (IsKeyDown(KEY_W)) fractalArgs->offsetY -= 0.01f * fractalArgs->zoom;
//             if (IsKeyDown(KEY_S)) fractalArgs->offsetY += 0.01f * fractalArgs->zoom;
//         }
//         else{
//             if (IsKeyDown(KEY_A)) fractalArgs->offsetX -= 0.13f * fractalArgs->zoom;
//             if (IsKeyDown(KEY_D)) fractalArgs->offsetX += 0.13f * fractalArgs->zoom;
//             if (IsKeyDown(KEY_W)) fractalArgs->offsetY -= 0.13f * fractalArgs->zoom;
//             if (IsKeyDown(KEY_S)) fractalArgs->offsetY += 0.13f * fractalArgs->zoom;
//         }
//         if (IsKeyDown(KEY_R)) fractalArgs->MAX_ITERATIONS += 10;
//         if (IsKeyDown(KEY_F) && fractalArgs->MAX_ITERATIONS > 0) fractalArgs->MAX_ITERATIONS -= 10;

//         if (IsMouseButtonDown(MOUSE_BUTTON_LEFT)) {
//             Vector2 center {fractalArgs->screenWidth / 2, fractalArgs->screenHeight / 2};

//             fractalArgs->cReal = (GetMousePosition().x - center.x) * .005;
//             fractalArgs->cImag = (GetMousePosition().y - center.y) * .005;
//         }

//         for (int x = 0; x < fractalArgs->screenWidth; x+=100) {
//             std::thread t(CalculateFractal, fractalArgs, 0, 0, fractalArgs->screenWidth, fractalArgs->screenHeight);
//             threads.push_back(std::move(t));
//             threadCounter += 1;
//         }
//         for (std::thread& t : threads) {
//             t.join();
//         }
//         // std::thread t1(CalculateFractal, fractalArgs, 0, 0, 200, fractalArgs->screenHeight);
//         // std::thread t2(CalculateFractal, fractalArgs, 200, 0, 400, fractalArgs->screenHeight);
//         // std::thread t3(CalculateFractal, fractalArgs, 400, 0, 600, fractalArgs->screenHeight);
//         // std::thread t4(CalculateFractal, fractalArgs, 600, 0, 800, fractalArgs->screenHeight);

//         // t1.join();
//         // t2.join();
//         // t3.join();
//         // t4.join();

//         BeginDrawing();

//         ClearBackground(BLACK);

//         for (Pixel item : fractalArgs->allPixels) {
//             DrawPixel(item.x,item.y,item.color);
//         }

//         // DrawTexture(target.texture, 0, 0, WHITE);

//         DrawFPS(100,100);

//         EndDrawing();

//         threads.clear();
//         fractalArgs->allPixels.clear();
//         // std::cout << "zoom: " << fractalArgs->zoom << " xoffset: " << fractalArgs->offsetX << " yoffset: " << fractalArgs->offsetY << " cReal: " << fractalArgs->cReal << " cImag: " << fractalArgs->cImag << " Max It: " << fractalArgs->MAX_ITERATIONS << std::endl;
//     }

//     CloseWindow();
//     return 0;
// }
