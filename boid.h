#ifndef BOID_H
#define BOID_H
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
void getSeparation(Boid * boid,Boid boids[],int nbBoid);
void boidAlign(Boid * boid, Boid boids[], int nbBoid);
void avoidBorder(Boid * boid);
void boidCohesion(Boid * boid, Boid boids[],int nbBoid);

#endif
