/*
    0: 40x20
    1: 60x20
    2: 80x20
    3: 100x20
    4: 20x40
    5: 20x60
    6: 20x80
    7: 20x100
    8: blank
*/

#define GRID_ROWS SCREEN_HEIGHT/100
#define GRID_COLS SCREEN_WIDTH/100

#include "raylib.h"

const int SCREEN_WIDTH{1280}, SCREEN_HEIGHT{720}; // Screen Dimensions
const char* SCREEN_TITLE{"Escape the Hunter"}; // Screen Title

double ballPosX{SCREEN_WIDTH / 2}, ballPosY{SCREEN_HEIGHT / 2};   // Ball Position
const double BALL_RADIUS{20};  // Ball Radius
const double BALL_VELOCITY{5}; // Ball Velocity

double hunterPosX{100}, hunterPosY{100};    // Hunter Position
double HUNTER_WIDTH{40}, HUNTER_HEIGHT{40};   // Hunter Dimensions
double HUNTER_VELOCITY{2};   // Hunter Velocity

const Color BALL_COLOR{WHITE}; // Ball Color
const Color SCREEN_BACKGROUND_COLOR = {0, 0, 0, 255};    // Background Color
const Color HUNTER_COLOR{PINK};   // Hunter Color

const int MAX_FPS{60};  // Maximum Frames Per Second 

bool grid[SCREEN_HEIGHT/100][SCREEN_WIDTH/100]; // Grid for Obstacles
