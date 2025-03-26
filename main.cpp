#include "raylib.h"
#include "variables.h"

#include <ctime>
#include <cstdlib>

void generateGrid() {
    for (int i = 0; i < SCREEN_HEIGHT/100; i++) {
        for (int j = 0; j < SCREEN_WIDTH/100; j++) {
            // Each cell has 10% chance of being an obstacle
            if (rand() % 100 < 10) {
                grid[i][j] = true;
            }
        }
    }
}

int main(void) {
    srand(time(0)); // Seed for random number generator

    generateGrid(); // Generate Grid
    InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, SCREEN_TITLE);  // Initialize Window
    SetTargetFPS(MAX_FPS);  // Set Target FPS

    while (WindowShouldClose() == false) {
        BeginDrawing(); // Begin Drawing

        bool is_game_running = true; // Player and hunter are visible by default

        bool collision_with_hunter = 
            (ballPosX + BALL_RADIUS >= hunterPosX) && 
            (ballPosX - BALL_RADIUS <= hunterPosX + HUNTER_WIDTH) && 
            (ballPosY + BALL_RADIUS >= hunterPosY) && 
            (ballPosY - BALL_RADIUS <= hunterPosY + HUNTER_HEIGHT)
        ;

        // End Game
        if (collision_with_hunter) {
            ClearBackground(SCREEN_BACKGROUND_COLOR);   // Clear Background with specific bgcolor
            
            is_game_running = false; // Remove ball and hunter from screen

            DrawText("Game Over", SCREEN_WIDTH/2 - 100, SCREEN_HEIGHT/2, 60, RED);  // Draw Text "Game Over" on the screen
            DrawText("EXIT [ESC]", SCREEN_WIDTH/2 - 100, SCREEN_HEIGHT/2 + 100, 40, BLUE); // Draw Text "EXIT" on the screen
            DrawText("RESTART [ENTER]", SCREEN_WIDTH/2 - 100, SCREEN_HEIGHT/2 + 150, 40, BLUE);  // Draw Text "RESTART" on the screen
            
            if (IsKeyPressed(KEY_ESCAPE)) break;   // Break the loop if ESC key is pressed
            if (IsKeyPressed(KEY_ENTER)) {
                ballPosX = SCREEN_WIDTH / 2;    // Reset Ball Position X
                ballPosY = SCREEN_HEIGHT / 2;   // Reset Ball Position Y

                hunterPosX = 100;   // Reset Hunter Position X
                hunterPosY = 100;   // Reset Hunter Position Y

                is_game_running = true;
            };   // Restart the game if Enter key is pressed
        }

        // Main Game
        if (is_game_running) {

            // Define coordinates of the ball in 100x100 px grid format
            int ballGridX = ballPosX / 100;
            int ballGridY = ballPosY / 100;

            // Ball Movement
            if ((IsKeyDown(KEY_RIGHT) || IsKeyDown(KEY_D)) && (ballPosX + BALL_RADIUS) < SCREEN_WIDTH) {
                int nextGridX = (int)((ballPosX + BALL_VELOCITY + BALL_RADIUS) / 100);
                if (nextGridX < SCREEN_WIDTH / 100 && ballGridY >= 0 && ballGridY < SCREEN_HEIGHT / 100 && grid[ballGridY][nextGridX]) {
                    // Collision detected to the right
                    ballPosX = nextGridX * 100 - BALL_RADIUS - 1;
                } else if (ballPosX + BALL_RADIUS < SCREEN_WIDTH) {
                    ballPosX += BALL_VELOCITY;
                }
            };
            if ((IsKeyDown(KEY_LEFT) || IsKeyDown(KEY_A)) && (ballPosX - BALL_RADIUS) > 0) {
                int nextGridX = (int)((ballPosX - BALL_VELOCITY - BALL_RADIUS) / 100);
                if (nextGridX >= 0 && ballGridY >= 0 && ballGridY < SCREEN_HEIGHT / 100 && grid[ballGridY][nextGridX]) {
                    // Collision detected to the left
                    ballPosX = (nextGridX + 1) * 100 + BALL_RADIUS + 1;
                } else if (ballPosX - BALL_RADIUS > 0) {
                    ballPosX -= BALL_VELOCITY;
                }
            };
            if ((IsKeyDown(KEY_UP) || IsKeyDown(KEY_W)) && (ballPosY - BALL_RADIUS) > 0) {
                int nextGridY = (int)((ballPosY - BALL_VELOCITY - BALL_RADIUS) / 100);
                if (nextGridY >= 0 && ballGridX >= 0 && ballGridX < SCREEN_WIDTH / 100 && grid[nextGridY][ballGridX]) {
                    // Collision detected upwards
                    ballPosY = (nextGridY + 1) * 100 + BALL_RADIUS + 1;
                } else if (ballPosY - BALL_RADIUS > 0) {
                    ballPosY -= BALL_VELOCITY;
                }
            };
            if ((IsKeyDown(KEY_DOWN) || IsKeyDown(KEY_S)) && (ballPosY + BALL_RADIUS) < SCREEN_HEIGHT) {
                int nextGridY = (int)((ballPosY + BALL_VELOCITY + BALL_RADIUS) / 100);
                if (nextGridY < SCREEN_HEIGHT / 100 && ballGridX >= 0 && ballGridX < SCREEN_WIDTH / 100 && grid[nextGridY][ballGridX]) {
                    // Collision detected downwards
                    ballPosY = nextGridY * 100 - BALL_RADIUS - 1;
                } else if (ballPosY + BALL_RADIUS < SCREEN_HEIGHT) {
                    ballPosY += BALL_VELOCITY;
                }
            };

            // Hunter AI Movement (following the ball) with Wall Collision and Basic Unsticking

            bool collidedHorizontal = false;
            bool collidedVertical = false;
            int unstickTimer = 0; // Timer to try a different direction

            if (ballPosX < (hunterPosX + (HUNTER_WIDTH / 2))) {
                int nextHunterMinGridX = (int)((hunterPosX - HUNTER_VELOCITY) / 100);
                bool canMove = true;
                for (int y = (int)(hunterPosY / 100); y <= (int)((hunterPosY + HUNTER_HEIGHT - 1) / 100); y++) {
                    if (nextHunterMinGridX >= 0 && y >= 0 && y < SCREEN_HEIGHT / 100 && grid[y][nextHunterMinGridX]) {
                        hunterPosX = (nextHunterMinGridX + 1) * 100;
                        collidedHorizontal = true;
                        canMove = false;
                        break;
                    }
                }
                if (canMove) hunterPosX -= HUNTER_VELOCITY;
                if (hunterPosX < 0) hunterPosX = 0;
            } else if (ballPosX > (hunterPosX + (HUNTER_WIDTH / 2))) {
                int nextHunterMaxGridX = (int)((hunterPosX + HUNTER_WIDTH + HUNTER_VELOCITY) / 100);
                bool canMove = true;
                for (int y = (int)(hunterPosY / 100); y <= (int)((hunterPosY + HUNTER_HEIGHT - 1) / 100); y++) {
                    if (nextHunterMaxGridX < SCREEN_WIDTH / 100 && y >= 0 && y < SCREEN_HEIGHT / 100 && grid[y][nextHunterMaxGridX]) {
                        hunterPosX = nextHunterMaxGridX * 100 - HUNTER_WIDTH;
                        collidedHorizontal = true;
                        canMove = false;
                        break;
                    }
                }
                if (canMove) hunterPosX += HUNTER_VELOCITY;
                if (hunterPosX > SCREEN_WIDTH - HUNTER_WIDTH) hunterPosX = SCREEN_WIDTH - HUNTER_WIDTH;
            }

            if (ballPosY < (hunterPosY + (HUNTER_HEIGHT / 2))) {
                int nextHunterMinGridY = (int)((hunterPosY - HUNTER_VELOCITY) / 100);
                bool canMove = true;
                for (int x = (int)(hunterPosX / 100); x <= (int)((hunterPosX + HUNTER_WIDTH - 1) / 100); x++) {
                    if (nextHunterMinGridY >= 0 && x >= 0 && x < SCREEN_WIDTH / 100 && grid[nextHunterMinGridY][x]) {
                        hunterPosY = (nextHunterMinGridY + 1) * 100;
                        collidedVertical = true;
                        canMove = false;
                        break;
                    }
                }
                if (canMove) hunterPosY -= HUNTER_VELOCITY;
                if (hunterPosY < 0) hunterPosY = 0;
            } else if (ballPosY > (hunterPosY + (HUNTER_HEIGHT / 2))) {
                int nextHunterMaxGridY = (int)((hunterPosY + HUNTER_HEIGHT + HUNTER_VELOCITY) / 100);
                bool canMove = true;
                for (int x = (int)(hunterPosX / 100); x <= (int)((hunterPosX + HUNTER_WIDTH - 1) / 100); x++) {
                    if (nextHunterMaxGridY < SCREEN_HEIGHT / 100 && x >= 0 && x < SCREEN_WIDTH / 100 && grid[nextHunterMaxGridY][x]) {
                        hunterPosY = nextHunterMaxGridY * 100 - HUNTER_HEIGHT;
                        collidedVertical = true;
                        canMove = false;
                        break;
                    }
                }
                if (canMove) hunterPosY += HUNTER_VELOCITY;
                if (hunterPosY > SCREEN_HEIGHT - HUNTER_HEIGHT) hunterPosY = SCREEN_HEIGHT - HUNTER_HEIGHT;
            }

            // Basic Unsticking Logic
            if ((collidedHorizontal || collidedVertical) && unstickTimer <= 0) {
                if (rand() % 100 < 30) { // 30% chance to try a perpendicular move
                    if (collidedHorizontal) {
                        if (ballPosY < hunterPosY) {
                            hunterPosY -= HUNTER_VELOCITY; // Try moving up
                        } else {
                            hunterPosY += HUNTER_VELOCITY; // Try moving down
                        }
                    } else if (collidedVertical) {
                        if (ballPosX < hunterPosX) {
                            hunterPosX -= HUNTER_VELOCITY; // Try moving left
                        } else {
                            hunterPosX += HUNTER_VELOCITY; // Try moving right
                        }
                    }
                    unstickTimer = 20; // Small delay before trying to unstick again
                }
            }

            if (unstickTimer > 0) {
                unstickTimer--;
            }
           
            DrawCircle(ballPosX, ballPosY, BALL_RADIUS, BALL_COLOR);    // Draw Ball on the screen
            DrawRectangle(hunterPosX, hunterPosY, HUNTER_WIDTH, HUNTER_HEIGHT, HUNTER_COLOR);   // Draw Hunter on the screen
         
            // Draw Grid
            for (int i = 0; i < GRID_ROWS; i++) {
                for (int j = 0; j < GRID_COLS; j++) {
                    if (grid[i][j]) {
                        // Calculate the screen position of the grid cell
                        int x = j * 100;
                        int y = i * 100;

                        // Draw a rectangle to represent the obstacle
                        DrawRectangle(x, y, 100, 100, DARKGRAY);
                    }
                }
            }
        }

        ClearBackground(SCREEN_BACKGROUND_COLOR);   // Clear Background with specific bgcolor
        EndDrawing();   // End Drawing
    }

    CloseWindow();  // Close window and OpenGL context
    
    return 0;
}