#include <stddef.h>
#include "raylib.h"
#include "raymath.h"
#include "rlgl.h"
#include "boid.h"

//Cap the speed of the boid between their minSpeed and maxSpeed value
void speedCap(Boid * boid){
    if(Vector2Length(boid->velocity)<boid->minSpeed){
        boid->velocity = Vector2Scale(Vector2Normalize(boid->velocity),boid->minSpeed);
    }
    if(Vector2Length(boid->velocity)>boid->maxSpeed){
        boid->velocity = Vector2Scale(Vector2Normalize(boid->velocity),boid->maxSpeed);
    }
    
}

//Separate the boids that are too close
void getSeparation(Boid * boid,Boid boids[],int nbBoid){
    Vector2 close = {0,0};

    for(int i = 0; i < nbBoid; i++){
        if(CheckCollisionCircles(boid->position,boid->protectedRange,boids[i].position,2)){
            close = Vector2Subtract(boid->position,boids[i].position);
        }
    } 

    boid->velocity = Vector2Add(boid->velocity,Vector2Scale(close,0.3)); 
}

//Match a boid velocity with the average of the others boid in his vision range
void boidAlign(Boid * boid, Boid boids[], int nbBoid){
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

//Make the boids turn when going beyond screen's border
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

//Orient the boid to the mass center of boids in visual range
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
