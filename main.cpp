#include <iostream>
#include <raylib.h>

#define PHYSAC_IMPLEMENTATION
#include "Libraries/physac.h"

#ifdef _WIN32
//#include <windows.h>
#ifdef _MSC_VER
extern "C" __declspec(dllexport) unsigned long NvOptimusEnablement = 0x00000001;
extern "C" __declspec(dllexport) unsigned long AmdPowerXpressRequestHighPerformance = 0x00000001;
#elif defined __GNUG__
extern "C" __attribute__ ((dllexport)) unsigned long NvOptimusEnablement = 0x00000001;
extern "C" __attribute__ ((dllexport)) unsigned long AmdPowerXpressRequestHighPerformance = 0x00000001;
#endif
#endif

int main () 
{  
    const int screenWidth = 1280;
    const int screenHeight = 720;
    const float speed = 0.5f;
    InitWindow(screenWidth, screenHeight, "Physics Test");
    InitPhysics(); // Initialize physics 
    SetTargetFPS(60); // Set default FPS

    float acc = 0.f;

    struct Player {
        Texture2D img;
        Rectangle rect;
        float scale;
        Vector2 position;
        float animSpeed;
        uint32_t frameNum;
        uint32_t animIndex;
    } player;

    player.img = LoadTexture("player.png");
    player.rect.x = 0.f;
    player.rect.y = 0.f;
    player.rect.width = player.img.width / 4;
    player.rect.height = player.img.height / 4;
    player.scale = 4;
    player.position.x = screenWidth / 2;
    player.position.y = screenHeight / 2;
    player.animSpeed = 1;
    player.animIndex = 0;

    Rectangle destRect;
    destRect.x = screenWidth / 2;
    destRect.y = screenHeight / 2;
    destRect.width = player.rect.width * player.scale;
    destRect.height = player.rect.height * player.scale;

    PhysicsBody floor = CreatePhysicsBodyRectangle((Vector2) {screenWidth / 2, screenHeight}, screenWidth, 100, 10);
    PhysicsBody platformLeft = CreatePhysicsBodyRectangle((Vector2) {screenWidth * 0.25f, screenHeight * 0.6f}, screenWidth * 0.25, 10, 10);
    PhysicsBody platformRight = CreatePhysicsBodyRectangle((Vector2) {screenWidth * (600.0f / 800.0f), screenHeight * (270.0f / 450.0f)}, screenWidth * (200.0f / 800.0f), 10, 10);

    floor -> enabled = false;
    platformLeft -> enabled = false;
    platformRight -> enabled = false;

    // Creating movement physics body
    PhysicsBody body = CreatePhysicsBodyRectangle((Vector2) {screenWidth / 2, screenHeight / 2}, 50, 50, 1);
    body -> freezeOrient = true;

    while (!WindowShouldClose())
    {
        if (IsKeyDown(KEY_W))
            //body -> velocity.x = speed;
            player.position.y -= 500 * GetFrameTime();
        if (IsKeyDown(KEY_A))
            //body -> velocity.x = -speed;
            player.position.x -= 500 * GetFrameTime();
        if (IsKeyDown(KEY_S))
            //body -> velocity.x = -speed;
            player.position.y += 500 * GetFrameTime();
        if (IsKeyDown(KEY_D))
            //body -> velocity.x = speed;
            player.position.x += 500 * GetFrameTime();

        //if (IsKeyDown(KEY_UP) && body -> isGrounded)
        //    body -> velocity.y = -speed * 4;

        if (IsKeyPressed(KEY_RIGHT_BRACKET))
        {
            player.scale += 1;
        }

        if (IsKeyPressed(KEY_LEFT_BRACKET))
        {
            player.scale -= 1;
        }

        if (IsKeyPressed(KEY_I))
        {
            player.animSpeed -= 1;
            if (player.animSpeed < 0) player.animSpeed = 0;
        }

        if (IsKeyPressed(KEY_O))
        {
            player.animSpeed += 1;
            if (player.animSpeed < 0) player.animSpeed = 0;
        }

        if (IsKeyPressed(KEY_U))
        {
            player.animIndex += 1;
            player.animIndex %= 4;
        }

        acc += GetFrameTime();

        if (acc >= 1.f/player.animSpeed)
        {
            player.frameNum++;
            acc = 0.f;
        }
        player.frameNum %= 4;
        player.rect.x = player.rect.width * player.frameNum;
        player.rect.y = player.rect.height * player.animIndex;

        destRect.x = player.position.x;
        destRect.y = player.position.y;
        destRect.width = player.rect.width * player.scale;
        destRect.height = player.rect.height * player.scale;

        BeginDrawing();

        ClearBackground({80, 50, 60, 255});
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

        DrawTexturePro(player.img, player.rect, destRect, {destRect.width/2, destRect.height/2}, 0.f, WHITE);
        //DrawTextureRec(player.img, player.rect, {(float)(screenWidth/2), (float)(screenHeight/2)}, WHITE);

        EndDrawing();
    }
    
    ClosePhysics();
    CloseWindow();
    return 0;
}