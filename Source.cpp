#include <iostream>
#include <raylib.h>
#include <string>

int cpuScore = 0;
int playerScore = 0;

class Ball
{
public:
    float x, y;
    float speedX, speedY;
    float radius;

    void Draw()
    {
        DrawCircle(x, y, radius, WHITE);
    }

    void update()
    {
        x += speedX;
        y += speedY;

        if (y + radius >= GetScreenHeight() || y - radius <= 0)
        {
            speedY *= -1;
        }
        if (x + radius >= GetScreenWidth())
        {
            speedX *= -1;
            cpuScore++;
            ResetBall();
        }
        if (x - radius <= 0) {
            speedX *= -1;
            playerScore++;
            ResetBall();
        }
    }

    void ResetBall()
    {
        x = GetScreenWidth() / 2;
        y = GetScreenHeight() / 2;
    }
};

class Paddle
{
protected:
    void limitMovement()
    {
        if (y <= 0)
        {
            y = 0;
        }
        if (y + height >= GetScreenHeight())
        {
            y = GetScreenHeight() - height;
        }
    }
public:
    int speed;
    float x;
    float y;
    float width;
    float height;

    void Draw()
    {
        DrawRectangle(x, y, width, height, WHITE);
    }

    void Update() {
        if (IsKeyDown(KEY_UP))
        {
            y -= speed;
        }
        if (IsKeyDown(KEY_DOWN))
        {
            y += speed;
        }
        limitMovement();
    }
};

class CPU : public Paddle
{
public:
    void update(int ballY)
    {
        if (y + height / 2 > ballY)
        {
            y -= speed;
        }
        if (y + height < ballY)
        {
            y += speed;
        }
        limitMovement();
    }
};

Ball ball;
Paddle player;
CPU cpu;

int main()
{
    const int windowHeight = 600;
    const int windowWidth = 800;

    const int paddleHeight = 120;
    const int paddleWidth = 25;

    ball.radius = 20;
    ball.x = windowWidth / 2;
    ball.y = windowHeight / 2;
    ball.speedX = 6;
    ball.speedY = 8;

    player.width = float(25);
    player.height = float(120);
    player.x = float(windowWidth - player.width - 10);
    player.y = float(windowHeight / 2 - player.height / 2);
    player.speed = 7;
    playerScore = 0;

    cpu.width = 25;
    cpu.height = 120;
    cpu.speed = 6;
    cpu.x = 10;
    cpu.y = (windowWidth / 2) - cpu.height;
    cpuScore = 0;

    SetTargetFPS(60);
    InitWindow(windowWidth, windowHeight, "Pong");

    while (!WindowShouldClose())
    {
        BeginDrawing();

        // Updating
        ball.update();
        player.Update();
        cpu.update(ball.y);

        // Checking for Collisions
        if (CheckCollisionCircleRec(Vector2{ ball.x, ball.y }, ball.radius, Rectangle{ player.x, player.y, player.width, player.height }))
        {
            ball.speedX *= -1;
        }
        if (CheckCollisionCircleRec(Vector2{ ball.x, ball.y }, ball.radius, Rectangle{ cpu.x, cpu.y, cpu.width, cpu.height }))
        {
            ball.speedX *= -1;
        }
        // Drawing
        ClearBackground(BLACK);
        DrawLine(windowWidth / 2, 0, windowWidth / 2, windowHeight, WHITE);
        ball.Draw();
        player.Draw();
        cpu.Draw();
        DrawText(TextFormat("%i", cpuScore), (windowWidth / 4) - 20, 20, 80, WHITE);
        DrawText(TextFormat("%i", playerScore), (windowWidth / 4) * 3, 20, 80, WHITE);

        EndDrawing();
    }

    CloseWindow();
    return 0;
}