/*******************************************************************************************
*
*   raylib [core] example - 3d camera first person
*
*   Example originally created with raylib 1.3, last time updated with raylib 1.3
*
*   Example licensed under an unmodified zlib/libpng license, which is an OSI-certified,
*   BSD-like license that allows static linking with closed source software
*
*   Copyright (c) 2015-2024 Ramon Santamaria (@raysan5)
*
********************************************************************************************/

#include <stddef.h>
#include "raylib.h"
#include "raymath.h"
#include "rlgl.h"
#include "boid.h"

//------------------------------------------------------------------------------------
// Program main entry point
//------------------------------------------------------------------------------------
int main(void)
{
    // Initialization
    //--------------------------------------------------------------------------------------
    const int screenWidth = 1000;
    const int screenHeight = 700;
    const int nbBoid = 200;

    InitWindow(screenWidth, screenHeight, "Boids");

    SetTargetFPS(30);                   // Set our game to run at 60 frames-per-second 
    
    Vector2 screenBorder[] = {
        (Vector2){100,50},
        (Vector2){screenWidth-100,50},
        (Vector2){screenWidth-100,screenHeight-50},
        (Vector2){100,screenHeight-50}
    };

    Boid boids[nbBoid];
    
    for(int i = 0; i<nbBoid; i++){
        Boid boid = {
            (Vector2){GetRandomValue(100,screenWidth-100),GetRandomValue(50,screenHeight-50)},
            (Vector2){0,0},
            0,
            10,
            60,
            150,
            130};

        boids[i] = boid;
    }

    // Main game loop
    while (!WindowShouldClose())        // Detect window close button or ESC key
    {
        for(int i=0;i<nbBoid;i++){
            getSeparation(&boids[i],boids,nbBoid);
            boidAlign(&boids[i],boids,nbBoid);
            boidCohesion(&boids[i],boids,nbBoid);
            avoidBorder(&boids[i]);
            speedCap(&boids[i]);

            float delta = GetFrameTime();
            boids[i].position = Vector2Add(boids[i].position,Vector2Scale(boids[i].velocity,delta));
        }

        //----------------------------------------------------------------------------------
        BeginDrawing();
            ClearBackground(RAYWHITE);

            DrawLineV(screenBorder[0],screenBorder[1],BLACK);
            DrawLineV(screenBorder[1],screenBorder[2],BLACK);
            DrawLineV(screenBorder[2],screenBorder[3],BLACK);
            DrawLineV(screenBorder[3],screenBorder[0],BLACK);

            for(int i = 0; i<nbBoid; i++){
                DrawPoly((Vector2){boids[i].position.x,boids[i].position.y},3,5,boids[i].rotation,RED); 
            }
        EndDrawing();
        //----------------------------------------------------------------------------------
    }

    // De-Initialization
    //--------------------------------------------------------------------------------------
    CloseWindow();        // Close window and OpenGL context
    //--------------------------------------------------------------------------------------

    return 0;
}
