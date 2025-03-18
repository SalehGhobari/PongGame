#include <iostream>
#include "raylib.h"




enum gameState
{
    MENU,
    GAME,
    EXIT
};




int main()
{
    std::srand(static_cast<unsigned int>(std::time(0)));

    // Initialization
//--------------------------------------------------------------------------------------
    // constants
    const int screenWidth = 1920;
    const int screenHeight = 1080;
    const float playerSpeed = 700.0f;
    const float ballRadius = 8.0f;
    bool resetBall = false;
    bool exitRequest = false;
    bool exit = false;
    float ballSpeedMultiplier = 500.0f;
    float ballVx = ballSpeedMultiplier * (rand() % 2 == 0 ? -1 : 1);
    float ballVy = ballSpeedMultiplier * (rand() % 2 == 0 ? -1 : 1);
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
    Vector2 ballCenter = { screenWidth / 2.0, screenHeight / 2.0};
    Vector2 ballReset = { screenWidth / 2.0, screenHeight / 2.0 };

    // Menu Rectangles
    Rectangle playButton = { screenWidth / 2.0 - 100, screenHeight / 2.0 - 25, 200, 50 };
    Rectangle exitButton = { screenWidth / 2.0 - 100, screenHeight / 2.0 + 100, 200, 50 };
    
    gameState CurrentState = MENU;

    
    

    InitWindow(screenWidth, screenHeight, "PongGame");
    // Measure text for exit confirmation
    Vector2 textMeasurement = MeasureTextEx(GetFontDefault(), "Are you sure you want to exit ?", 30, 0);
    ToggleFullscreen();
    //--------------------------------------------------------------------------------------
    InitAudioDevice();
    Sound paddleHit = LoadSound("../Assets/paddleHit.mp3");
    Sound pointWon = LoadSound("../Assets/Point.mp3");
    // Main game loop
    while (CurrentState != EXIT)    // Detect window close button or ESC key
    {

        float deltaTime = GetFrameTime();
        if (CurrentState == MENU)
        {
            Vector2 mousePos = GetMousePosition();
            if (CheckCollisionPointRec(mousePos, playButton) && IsMouseButtonPressed(MOUSE_BUTTON_LEFT))
            {
                CurrentState = GAME;
            }
            if (CheckCollisionPointRec(mousePos, exitButton) && IsMouseButtonPressed(MOUSE_BUTTON_LEFT))
            {
                CurrentState = EXIT;
            }
            BeginDrawing();
            ClearBackground(BLACK);
            DrawText("PONG GAME", screenWidth / 2 - 80, 150, 60, PURPLE);
            DrawRectangleRec(playButton, LIGHTGRAY);
            DrawRectangleRec(exitButton, LIGHTGRAY);
            DrawText("PLAY", playButton.x + 70, playButton.y, 40, BLACK);
            DrawText("EXIT", exitButton.x + 70, exitButton.y, 40, BLACK);
            EndDrawing();
        }
        // Update
        //----------------------------------------------------------------------------------
        // TODO: Update your variables here
        else if (CurrentState == GAME)
        {
            if (WindowShouldClose() || IsKeyPressed(KEY_ESCAPE))
            {
                exitRequest = true;
            }
            if (exitRequest)
            {
                if (IsKeyPressed(KEY_Y))
                {
                    exit = true;
                    CurrentState = EXIT;
                }

                else if (IsKeyPressed(KEY_N))
                {
                    exitRequest = false;
                    CurrentState = GAME;
                }
            }
            if (!resetBall)
            {
                ballCenter.x += ballVx * deltaTime;
                ballCenter.y += ballVy * deltaTime;
            }

            if (!resetBall && CheckCollisionCircleRec(ballCenter, ballRadius, rightBorder))
            {
                leftPlayerScore += 1;
                PlaySound(pointWon);
                resetBall = true;
            }
            if (!resetBall && CheckCollisionCircleRec(ballCenter, ballRadius, leftBorder))
            {
                rightPlayerScore += 1;
                PlaySound(pointWon);
                resetBall = true;
            }
            if (resetBall)
            {
                ballCenter = ballReset;
                ballVx = ballSpeedMultiplier * (rand() % 2 == 0 ? -1 : 1);
                ballVy = ballSpeedMultiplier * (rand() % 2 == 0 ? -1 : 1);
                resetBall = false;
            }

            // CheckCollisionCircleRec(Vector2 center, float radius, Rectangle rec); 
            if (CheckCollisionCircleRec(ballCenter, ballRadius, rightPlayer) || CheckCollisionCircleRec(ballCenter, ballRadius, leftPlayer))
            {
                ballVx *= -1;
                PlaySound(paddleHit);
            }
            if (CheckCollisionCircleRec(ballCenter, ballRadius, topBorder) || CheckCollisionCircleRec(ballCenter, ballRadius, bottomBorder))
            {
                ballVy *= -1;
            }

            if (IsKeyDown(KEY_W) && !CheckCollisionRecs(leftPlayer, topBorder))
            {
                leftPlayer.y -= playerSpeed * deltaTime;
            }
            if (IsKeyDown(KEY_S) && !CheckCollisionRecs(leftPlayer, bottomBorder))
            {
                leftPlayer.y += playerSpeed * deltaTime;
            }
            if (IsKeyDown(KEY_UP) && !CheckCollisionRecs(rightPlayer, topBorder))
            {
                rightPlayer.y -= playerSpeed * deltaTime;
            }
            if (IsKeyDown(KEY_DOWN) && !CheckCollisionRecs(rightPlayer, bottomBorder))
            {
                rightPlayer.y += playerSpeed * deltaTime;
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
            DrawRectanglePro(leftPlayer, { 0, 0 }, 0, PURPLE);
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

            // Draw exit confirmation
            if (exitRequest)
            {
                //DrawText(const char *text, int posX, int posY, int fontSize, Color color); 
                DrawRectangle(screenWidth / 2 - textMeasurement.x / 2, screenHeight / 2.0 - textMeasurement.y / 2, textMeasurement.x + 186, textMeasurement.y, WHITE);
                DrawText("Are you sure you want to exit ? [Y/N]", screenWidth / 2 - textMeasurement.x / 2, screenHeight / 2.0 - textMeasurement.y / 2, 30, PURPLE);
                std::cout << textMeasurement.x << "\n";
            }

            EndDrawing();
        }
        //----------------------------------------------------------------------------------
    }

    
    // De-Initialization
    //--------------------------------------------------------------------------------------
    CloseWindow();        // Close window and OpenGL context
    //-
	return 0;
}