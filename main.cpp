#include <iostream>
#include "raylib.h"

int main()
{
    std::srand(static_cast<unsigned int>(std::time(0)));

    // Initialization
//--------------------------------------------------------------------------------------
    // constants
    const int screenWidth = 1920;
    const int screenHeight = 1080;
    const float playerSpeed = 7.0f;
    const float ballRadius = 8.0f;
    bool resetBall = false;
    float ballSpeedMultiplier = 5.0f;
    float ballVx = 3.5f * (rand() % 2 == 0 ? -1 : 1);
    float ballVy = 3.5f * (rand() % 2 == 0 ? -1 : 1);
    int leftPlayerScore = 0;
    int rightPlayerScore = 0;
    // Player rectangles
    Rectangle leftPlayer = { (float) 20, (float) screenHeight / 2, 8, 130 };
    Rectangle rightPlayer = { (float) screenWidth - 20, (float) screenHeight / 2 , 8, 130 };

    // Border rectangles to check for collisions
    Rectangle topBorder = { 0, 0, screenWidth, 0 };
    Rectangle bottomBorder = { 0, screenHeight, screenWidth, 0 };
    Rectangle rightBorder = { screenWidth, 0, 0, screenHeight };
    Rectangle leftBorder = { 0, 0 , 0, screenHeight };

    // Circle stuff
    Vector2 ballCenter = {screenWidth / 2.0, screenHeight / 2.0};
    Vector2 ballReset = { screenWidth / 2.0, screenHeight / 2.0 };

    InitWindow(screenWidth, screenHeight, "PongGame");
    ToggleFullscreen();
    SetTargetFPS(144);               // Set our game to run at 144 frames-per-second
    //--------------------------------------------------------------------------------------

    // Main game loop
    while (!WindowShouldClose())    // Detect window close button or ESC key
    {
        // Update
        //----------------------------------------------------------------------------------
        // TODO: Update your variables here
        if (!resetBall)
        {
            ballCenter.x += ballVx;
            ballCenter.y += ballVy;
        }

        if (!resetBall && CheckCollisionCircleRec(ballCenter, ballRadius, rightBorder))
        {
            leftPlayerScore += 1;
            resetBall = true;
        }
        if (!resetBall && CheckCollisionCircleRec(ballCenter, ballRadius, leftBorder))
        {
            rightPlayerScore += 1;
            resetBall = true;
        }
        if (resetBall)
        {
            ballCenter = ballReset;
            ballVx = 3.5f * (rand() % 2 == 0 ? -1 : 1);
            ballVy = 3.5f * (rand() % 2 == 0 ? -1 : 1);
            resetBall = false;
        }

        // CheckCollisionCircleRec(Vector2 center, float radius, Rectangle rec); 
        if (CheckCollisionCircleRec(ballCenter, ballRadius, rightPlayer) || CheckCollisionCircleRec(ballCenter, ballRadius, leftPlayer))
        {
            ballVx *= -1;
            ballVy *= -1;
        }
        if (CheckCollisionCircleRec(ballCenter, ballRadius, topBorder) || CheckCollisionCircleRec(ballCenter, ballRadius, bottomBorder))
        {
            ballVy *= -1;
        }

        if (IsKeyDown(KEY_W) && !CheckCollisionRecs(leftPlayer, topBorder))
        {
            leftPlayer.y -= playerSpeed;
        }
        if (IsKeyDown(KEY_S) && !CheckCollisionRecs(leftPlayer, bottomBorder))
        {
            leftPlayer.y += playerSpeed;
        }
        if (IsKeyDown(KEY_UP) && !CheckCollisionRecs(rightPlayer, topBorder))
        {
            rightPlayer.y -= playerSpeed;
        }
        if (IsKeyDown(KEY_DOWN) && !CheckCollisionRecs(rightPlayer, bottomBorder))
        {
            rightPlayer.y += playerSpeed;
        }
        //----------------------------------------------------------------------------------

        // Draw
        //----------------------------------------------------------------------------------
        BeginDrawing();
        ClearBackground(BLACK);
        DrawFPS(0, 0);
        // Draw Seperating line

        DrawLine(screenWidth / 2, 0, screenWidth / 2, screenHeight, PURPLE);

        // Draw Player Paddles
        DrawRectanglePro(leftPlayer, {0, 0}, 0, PURPLE);
        DrawRectanglePro(rightPlayer, { 0, 0 }, 0, PURPLE);

        // Draw Border Rectangles
        DrawRectanglePro(topBorder, { 0, 0 }, 0, PURPLE);
        DrawRectanglePro(bottomBorder, { 0, 0 }, 0, PURPLE);
        DrawRectanglePro(rightBorder, { 0, 0 }, 0, PURPLE);
        DrawRectanglePro(leftBorder, { 0, 0 }, 0, PURPLE);

        // Draw Ball
        DrawCircleV(ballCenter, ballRadius, ORANGE);

        // Draw Scores
        DrawText(TextFormat("%d", rightPlayerScore), 1440, 100, 50, PURPLE);
        DrawText(TextFormat("%d", leftPlayerScore), 480, 100, 50, PURPLE);
        
        EndDrawing();
        //----------------------------------------------------------------------------------
    }

    // De-Initialization
    //--------------------------------------------------------------------------------------
    CloseWindow();        // Close window and OpenGL context
    //-
	return 0;
}