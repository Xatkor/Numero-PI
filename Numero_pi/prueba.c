#include <stdio.h>
#include "raylib.h"
#include "include/raymath.h"
#include <math.h>
#include <stdlib.h>

#include "include/rlgl.h"

#define GLSL_VERSION 330

#define MICOLOR CLITERAL(Color){ 0, 245, 245, 255} // {R, G, B, alpha} -- {0-255, 0-255, 0-255, 0-255}
#define MAX_POINTS 25000

typedef struct points
{
    int x;
    int y;
    float radii;
    Color color;
} point;

void GeneratePoints(point *,int rectangleX, int rectangleY, float circle_radii);
bool is_in_circle(point *, float circle_radii);

int main(void)
{
    const int screenWidth = 1200; 
    const int screenHeight = 720;

    InitWindow(screenWidth, screenHeight, "Numero PI");

    int FPS_initial = 1;
    SetTargetFPS(FPS_initial);               // Set our game to run at 60 frames-per-second

    bool pause = false;
    int framesCounter = 0;
    int count = 0;
    float pi;

    point p;
    int puntos_dentro = 0;

    RenderTexture2D cumulativeDots = LoadRenderTexture(screenWidth, screenHeight);

    // ------------ Cargar el shader del dardo
    Shader circle_shader = LoadShader(NULL, "./circle.fs");

    // Main game loop
    while (!WindowShouldClose())    // Detect window close button or ESC key
    {
        // Update
        //----------------------------------------------------------------------------------

        if (FPS_initial < 60) {
            FPS_initial += 1;
            SetTargetFPS(FPS_initial);
        } else {
            SetTargetFPS(60);
        }

        float circle_radii = (screenHeight - 100) / 2;
        Color circle_color = GREEN;

        if (IsKeyPressed(KEY_SPACE)) pause = !pause;

        if (!pause && count < MAX_POINTS)
        {
            GeneratePoints(&p, 10, 10, circle_radii);
            count += 1;
            if (is_in_circle(&p, circle_radii)){
                puntos_dentro += 1;
            }
            pi = 4 * (float)puntos_dentro / count;
        }

        int current_time = (int)GetTime();
        //int h = current_time / 3600;
        int m = (current_time / 60) % 60;
        int s = current_time % 60;

        //----------------------------------------------------------------------------------

        // Draw
        //----------------------------------------------------------------------------------
        BeginTextureMode(cumulativeDots); 
            DrawCircleGradient(p.x , screenHeight - p.y, 10, p.color, BLACK); // Usa OpenGL (0, 0) at bottom left 
        EndTextureMode();
        
        BeginDrawing();
        ClearBackground(BLACK);

        DrawRectangleLines(10, 10, circle_radii * 2, circle_radii * 2, RED);   // Cuadrado 
        DrawCircleLines(circle_radii + 10, circle_radii + 10, circle_radii, circle_color);    // Diana
        
        DrawText(TextFormat("Points in circle: %03i", puntos_dentro), circle_radii*2 + 100, 20, 20, GREEN); 
        DrawText(TextFormat("Total points: %03i", count), circle_radii*2 + 100, 50, 20, LIGHTGRAY); 

        DrawText(TextFormat("Pi: %f", pi), circle_radii*2 + 100, 80, 20, WHITE);
        DrawText(TextFormat("Valor teorico: %.09f", M_PI), circle_radii*2 + 100, 110, 20, LIGHTGRAY);
        DrawText(TextFormat("Error cometido: %.09f", fabs(pi - M_PI) / M_PI), circle_radii*2 + 100, circle_radii * 2 - 10, 20, LIGHTGRAY);

        DrawText(TextFormat("Elapsed time: %02u:%02u", m, s), circle_radii*2 + 100, 150, 20, LIGHTGRAY);

        DrawTexture(cumulativeDots.texture, 0, 0, RAYWHITE);

        // ----------- DARDOS SHADER -----------       
        float customRadius = 20.0f; // Define el radio personalizado
        int power = 3;
        SetShaderValue(circle_shader, GetShaderLocation(circle_shader, "radius"), &customRadius, SHADER_UNIFORM_FLOAT);
        SetShaderValue(circle_shader, GetShaderLocation(circle_shader, "power"), &power, SHADER_UNIFORM_FLOAT);
        Texture2D texture = { rlGetTextureIdDefault(), 1, 1, 1, PIXELFORMAT_UNCOMPRESSED_R8G8B8A8 };
        
        BeginShaderMode(circle_shader);
            Vector2 position = {
                p.x - customRadius,
                p.y - customRadius,
            };
            float hue = 180;
            float saturation = 1;
            float value = 1;
            Color color = ColorFromHSV(hue, saturation, value);
            DrawTextureEx(texture, position, 0, 2*customRadius, color);
        EndShaderMode();


        DrawFPS(700, 450);

        EndDrawing();
        //----------------------------------------------------------------------------------
    }

    // De-Initialization
    //--------------------------------------------------------------------------------------
    UnloadShader(circle_shader);
    UnloadRenderTexture(cumulativeDots);


    CloseWindow();        // Close window and OpenGL context
    //--------------------------------------------------------------------------------------
    return 0;
}

void GeneratePoints(point *p ,int rectangleX, int rectangleY, float circle_radii){
    p->x = GetRandomValue(rectangleX, circle_radii * 2 + 10);
    p->y = GetRandomValue(rectangleY, circle_radii * 2 + 10);
    p->radii = 2;
    int a = p->x;
    int b = p->y;

    if (is_in_circle(p, circle_radii)){
        p->color = GREEN;
    } else {
        p->color = RED;
    }
}

bool is_in_circle(point *p, float circle_radii){
    int a = p->x;
    int b = p->y;
    float centroX = circle_radii + 10;
    float centroY = circle_radii + 10;
    if ( (a - centroX) * (a - centroX) + (b - centroY) * (b - centroY) < (circle_radii) * (circle_radii)){
        return true;
    } else {
        return false;
    }
}
