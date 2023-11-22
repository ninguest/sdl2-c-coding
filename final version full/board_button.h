#ifndef BOARD_BUTTON_H
#define BOARD_BUTTON_H

#include "constants.h"
#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>

typedef struct {
    SDL_Rect rect;
    SDL_Texture* emptyTexture; //Texture for empty (state 0)
    SDL_Texture* xTexture;   // Texture for X symbol (state 1)
    SDL_Texture* oTexture;   // Texture for O symbol (state 2)
    int cellID;
    int isOccupied;
    SDL_Color bgColor;
} BoardButton;

// Initialize the game board buttons
// - boardButtons: An array of BoardButton structures representing the game board buttons
// - gridWidth: The width (number of columns) of the game board
// - gridHeight: The height (number of rows) of the game board
void initBoardButtons( SDL_Renderer* renderer, BoardButton* boardButtons);

// render the game board buttons
// - renderer: The SDL_Renderer used for rendering
// - boardButtons: An array of BoardButton structures representing the game board buttons
// - gridWidth: The width (number of columns) of the game board
// - gridHeight: The height (number of rows) of the game board
void renderBoardButtons(SDL_Renderer* renderer, BoardButton* boardButtons);

// Handle button clicks when the user interacts with the game board
// - boardButtons: An array of BoardButton structures representing the game board buttons
// - mouseX: The x-coordinate of the mouse click
// - mouseY: The y-coordinate of the mouse click
int handleBoardButtonClick(BoardButton* boardButtons, int mouseX, int mouseY,int currentplayer, int gameLogic,int gameOver);

int checkwin(BoardButton *boardButtons);

int checkDraw(BoardButton *boardButtons);

#endif