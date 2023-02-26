#include <iostream>
#include <raylib.h>

#define PHYSAC_IMPLEMENTATION
#include "Libraries/physac.h"

int main () 
{  
    const int screenWidth = 800;
    const int screenHeight = 450;
    const float speed = 0.5f;
    InitWindow(screenWidth, screenHeight, "Physics Test");
    InitPhysics(); // Initialize physics 
    SetTargetFPS(60); // Set default FPS

    PhysicsBody floor = CreatePhysicsBodyRectangle((Vector2) {screenWidth / 2, screenHeight}, screenWidth, 100, 10);
    PhysicsBody platformLeft = CreatePhysicsBodyRectangle((Vector2) {screenWidth * 0.25f, screenHeight * 0.6f}, screenWidth * 0.25, 10, 10);
    PhysicsBody platformRight = CreatePhysicsBodyRectangle((Vector2) {600, 270}, 200, 10, 10);

    floor -> enabled = false;
    platformLeft -> enabled = false;
    platformRight -> enabled = false;

    // Creating movement physics body
    PhysicsBody body = CreatePhysicsBodyRectangle((Vector2) {screenWidth / 2, screenHeight / 2}, 50, 50, 1);
    body -> freezeOrient = true;

    while (!WindowShouldClose())
    {
         if (IsKeyDown(KEY_RIGHT))
            body -> velocity.x = speed;
        else if (IsKeyDown(KEY_LEFT))
            body -> velocity.x = -speed;

        if (IsKeyDown(KEY_UP) && body -> isGrounded)
            body -> velocity.y = -speed * 4;

        BeginDrawing();

        ClearBackground(BLACK);
        DrawFPS(screenWidth - 90, screenHeight - 30);

        // Draw physics bodies
        int bodiesCount = GetPhysicsBodiesCount();
        for (int i = 0; i < bodiesCount; i++) 
        {
            PhysicsBody body = GetPhysicsBody(i);

            int vertexCount = GetPhysicsShapeVerticesCount(i);
            for (int j = 0; j < vertexCount; j++) 
            {
                Vector2 vertexA = GetPhysicsShapeVertex(body, j);

                int jj = (((j + 1) < vertexCount) ? (j + 1) : 0);
                Vector2 vertexB = GetPhysicsShapeVertex(body, jj);

                DrawLineV(vertexA, vertexB, WHITE);
            }
        }

        EndDrawing();
    }
    
    ClosePhysics();
    CloseWindow();
    return 0;
}