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

typedef struct{
        Vector2 position;
        Vector2 velocity;
        float rotation;
        float protectedRange;
        float visualRange;
        float maxSpeed;
        float minSpeed;
} Boid;

void speedCap(Boid * boid);
void GetSeparation(Boid * boid,Boid boids[],int nbBoid);
void BoidAlign(Boid * boid, Boid boids[], int nbBoid);
void avoidBorder(Boid * boid);
void boidCohesion(Boid * boid, Boid boids[],int nbBoid);

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
            //Separate boids
            GetSeparation(&boids[i],boids,nbBoid);
            //Matching Velocity between boids
            BoidAlign(&boids[i],boids,nbBoid);
            boidCohesion(&boids[i],boids,nbBoid);
            //Avoid screen border
            avoidBorder(&boids[i]);
            //Cap the speed of boid
            speedCap(&boids[i]);
            //Move the boid according to velocity
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

void speedCap(Boid * boid){
    
    //Speed Checker
    if(Vector2Length(boid->velocity)<boid->minSpeed){
        boid->velocity = Vector2Scale(Vector2Normalize(boid->velocity),boid->minSpeed);
    }
    if(Vector2Length(boid->velocity)>boid->maxSpeed){
        boid->velocity = Vector2Scale(Vector2Normalize(boid->velocity),boid->maxSpeed);
    }
    
}

//Check separation factor with other boids and with screen border
void GetSeparation(Boid * boid,Boid boids[],int nbBoid){
    Vector2 close = {0,0};

    for(int i = 0; i < nbBoid; i++){
        if(CheckCollisionCircles(boid->position,boid->protectedRange,boids[i].position,2)){
            close = Vector2Subtract(boid->position,boids[i].position);
        }
    } 

    boid->velocity = Vector2Add(boid->velocity,Vector2Scale(close,0.3)); 
}

//Check alignement with other boids
void BoidAlign(Boid * boid, Boid boids[], int nbBoid){
    int boidCount = 0;
    Vector2 velAvg = {0,0};

    for(int i = 0 ; i<nbBoid; i++){
        if(CheckCollisionCircles(boid->position,boid->visualRange,boids[i].position,1) && !CheckCollisionCircles(boid->position,boid->protectedRange,boid[i].position,1)){
            boidCount++;
            velAvg = Vector2Add(velAvg, boids[i].velocity);
        }
    }

    if(boidCount > 0){
        velAvg.x = velAvg.x / boidCount;
        velAvg.y = velAvg.y / boidCount;
    }
    
    boid->velocity = Vector2Add(boid->velocity,Vector2Scale(Vector2Subtract(velAvg,boid->velocity),0.1)); //2 = matching factor
}

void avoidBorder(Boid * boid){
    //Screen border
    int turnFactor = 10;
    if(boid->position.x < 100){
        boid->velocity.x = boid->velocity.x + turnFactor;
    }
    else if(boid->position.x > 900){
        boid->velocity.x = boid->velocity.x - turnFactor;
    }
    
    if(boid->position.y < 50){
        boid->velocity.y = boid->velocity.y + turnFactor;
    }
    else if(boid->position.y > 650){
        boid->velocity.y = boid->velocity.y - turnFactor;
    }
}

void boidCohesion(Boid * boid, Boid boids[],int nbBoid){
    float centeringFactor = 0.005;

    Vector2 posAvg = {0,0};
    int boidCount=0;

    for(int i = 0 ; i<nbBoid; i++){
        if(CheckCollisionCircles(boid->position,boid->visualRange,boids[i].position,1) && !CheckCollisionCircles(boid->position,boid->protectedRange,boid[i].position,1)){
            boidCount++;
            posAvg = Vector2Add(posAvg,boids[i].position);
        }
    }

    if(boidCount > 0){
        posAvg.x = posAvg.x/boidCount;
        posAvg.y = posAvg.y/boidCount;
    }

    boid->velocity = Vector2Add(boid->velocity,Vector2Scale(Vector2Subtract(posAvg,boid->position),centeringFactor));
}
