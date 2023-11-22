#include "board_button.h"
#include "constants.h"
#include <stdio.h>
#include <math.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_mixer.h>
#include <SDL2/SDL_ttf.h>
#include "insaneGameLogic.h"
#include "easyGameLogic.h"
#include "mediumGameLogic.h"

int checkwin(BoardButton *boardButtons);
// Function to initialize the game board buttons
void initBoardButtons(SDL_Renderer *renderer, BoardButton *boardButtons)
{

    // Load Empty, X and O image textures into every button so it can
    // be used according to isOccupied property
    SDL_Surface *xSurface = IMG_Load("./resource/img/x.png");
    SDL_Surface *oSurface = IMG_Load("./resource/img/o.png");
    SDL_Surface *emptySurface = IMG_Load("./resource/img/empty.png");

    // Position the grid in the center of the window
    int gridX = (WINDOW_WIDTH - GRID_SIZE) / 2;
    int gridY = (WINDOW_HEIGHT - GRID_SIZE) / 2;

    // Initialize buttons
    for (int row = 0; row < GRID_HEIGHT; row++)
    {
        for (int col = 0; col < GRID_WIDTH; col++)
        {
            int buttonIndex = row * GRID_WIDTH + col;

            // Set button position and size
            boardButtons[buttonIndex].rect.x = gridX + col * CELL_SIZE;
            boardButtons[buttonIndex].rect.y = gridY + row * CELL_SIZE;
            boardButtons[buttonIndex].rect.w = CELL_SIZE;
            boardButtons[buttonIndex].rect.h = CELL_SIZE;

            // Initialize other button properties
            boardButtons[buttonIndex].emptyTexture = SDL_CreateTextureFromSurface(renderer, emptySurface); // Initialize to empty texture
            boardButtons[buttonIndex].oTexture = SDL_CreateTextureFromSurface(renderer, oSurface);         // Initialize to o texture
            boardButtons[buttonIndex].xTexture = SDL_CreateTextureFromSurface(renderer, xSurface);         // Initialize to x texture
            boardButtons[buttonIndex].cellID = buttonIndex;                                                // Set cellID
            boardButtons[buttonIndex].isOccupied = 0;                                                      // Initialize to not occupied
            boardButtons[buttonIndex].bgColor = (SDL_Color){255, 255, 255, 255};                           // Background color (white)
        }
    }

    SDL_FreeSurface(xSurface);
    SDL_FreeSurface(oSurface);
    SDL_FreeSurface(emptySurface);
}

// Function to render the game board buttons on the screen
void renderBoardButtons(SDL_Renderer *renderer, BoardButton *boardButtons)
{

    // Set the color for grid lines (e.g., black)
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);

    // Render horizontal grid lines
    for (int row = 1; row < GRID_HEIGHT; row++)
    {
        int y = (WINDOW_HEIGHT - GRID_SIZE) / 2 + row * CELL_SIZE;
        SDL_RenderDrawLine(renderer, (WINDOW_WIDTH - GRID_SIZE) / 2, y, (WINDOW_WIDTH + GRID_SIZE) / 2, y);
    }

    // Render vertical grid lines
    for (int col = 1; col < GRID_WIDTH; col++)
    {
        int x = (WINDOW_WIDTH - GRID_SIZE) / 2 + col * CELL_SIZE;
        SDL_RenderDrawLine(renderer, x, (WINDOW_HEIGHT - GRID_SIZE) / 2, x, (WINDOW_HEIGHT + GRID_SIZE) / 2);
    }

    // Render each board button
    for (int i = 0; i < GRID_WIDTH * GRID_HEIGHT; i++)
    {
        // Set the background color
        SDL_SetRenderDrawColor(renderer, boardButtons[i].bgColor.r, boardButtons[i].bgColor.g, boardButtons[i].bgColor.b, boardButtons[i].bgColor.a);

        // Render the background rectangle
        SDL_RenderFillRect(renderer, &(boardButtons[i].rect));

        // Render the empty, X or O texture
        if (boardButtons[i].isOccupied == 0)
        {
            SDL_RenderCopy(renderer, boardButtons[i].emptyTexture, NULL, &(boardButtons[i].rect));
        }
        else if (boardButtons[i].isOccupied == 1)
        {
            SDL_RenderCopy(renderer, boardButtons[i].xTexture, NULL, &(boardButtons[i].rect));
        }
        else if (boardButtons[i].isOccupied == 2)
        {
            SDL_RenderCopy(renderer, boardButtons[i].oTexture, NULL, &(boardButtons[i].rect));
        }
    }

    SDL_RenderPresent(renderer);
}

int handleBoardButtonClick(BoardButton *boardButtons, int mouseX, int mouseY, int currentplayer, int gameLogic, int gameOver)
{
    int invalidMove = 0;
    // Iterate through the buttons to check if a button was clicked
    for (int i = 0; i < GRID_WIDTH * GRID_HEIGHT; i++)
    {

        if (SDL_PointInRect(&(SDL_Point){mouseX, mouseY}, &(boardButtons[i].rect)))
        {
            // Handle the button click based on the button's cellID
            if (boardButtons[i].isOccupied == 0 && gameOver == 0)
            {
                if (gameLogic == 3)
                {
                    if (currentplayer == 1)
                    {
                        boardButtons[i].isOccupied = currentplayer; // X
                        
                    }
                    else if (currentplayer == 2)
                    {
                        boardButtons[i].isOccupied = currentplayer; // O

                     
                    }
                }
                else if (gameLogic == 2)
                // Computer Insane 1 = Player
                // 2 = Computer
                {

                    if (currentplayer == 1 && gameOver == 0)
                    {

                        boardButtons[i].isOccupied = currentplayer; // X
                        char arr[3][3] = {
                            {0, 0, 0},
                            {0, 0, 0},
                            {0, 0, 0}};
                        for (int i = 0; i < GRID_WIDTH * GRID_HEIGHT; i++)
                        {
                            arr[i / 3][i % 3] = (boardButtons[i].isOccupied == 1) ? 1 : (boardButtons[i].isOccupied == 2) ? 2
                                                                                                                          : 0;

                        }
                        if (checkwin(boardButtons) == 0 && checkDraw(boardButtons) == 0)
                        {

                            int computermove = insaneComputerTurn(arr);
                            boardButtons[computermove].isOccupied = 2; // 0
                        }
                    }

                    // Set the appropriate image based on the current player (1 for X, 2 for O)

                    // Waiting for game logic part
                }
                else if (gameLogic == 1)
                {
                    if (currentplayer == 1 && gameOver == 0)
                    {

                        boardButtons[i].isOccupied = currentplayer; // X

                        char arr[3][3] = {
                            {0, 0, 0},
                            {0, 0, 0},
                            {0, 0, 0}};
                        for (int i = 0; i < GRID_WIDTH * GRID_HEIGHT; i++)
                        {
                            arr[i / 3][i % 3] = (boardButtons[i].isOccupied == 1) ? 1 : (boardButtons[i].isOccupied == 2) ? 2
                                                                                                                          : 0;

                        }
                        if (checkwin(boardButtons) == 0 && checkDraw(boardButtons) == 0)
                        {

                            int computermove = mediumComputerTurn(arr);
                            boardButtons[computermove].isOccupied = 2; // 0
                        }
                    }

                    // Set the appropriate image based on the current player (1 for X, 2 for O)

                }
                else if (gameLogic == 0)
                {

                    if (currentplayer == 1 && gameOver == 0)
                    {

                        boardButtons[i].isOccupied = currentplayer; // X
                        int easyarr[9] = {0, 0, 0, 0, 0, 0, 0, 0, 0};
                        for (int i = 0; i < GRID_WIDTH * GRID_HEIGHT; i++)
                        {
                            easyarr[i] = (boardButtons[i].isOccupied == 1) ? 1 : (boardButtons[i].isOccupied == 2) ? 2
                                                                                                                   : 0;
                          
                        }

                        if (checkwin(boardButtons) == 0 && checkDraw(boardButtons) == 0)
                        {

                            int computermove = easyComputerTurn(easyarr);
                            boardButtons[computermove].isOccupied = 2; // 0
                        }

                    }
                }
            }
            else
                invalidMove = 1;
        }
    }
    return invalidMove;
}
int checkwin(BoardButton *boardButtons)
{
    // Check rows
    for (int i = 0; i < GRID_HEIGHT; i++)
    {
        int rowStart = i * GRID_WIDTH;
        if (boardButtons[rowStart].isOccupied != 0 && boardButtons[rowStart].isOccupied == boardButtons[rowStart + 1].isOccupied && boardButtons[rowStart + 1].isOccupied == boardButtons[rowStart + 2].isOccupied)
        {
            return boardButtons[rowStart].isOccupied;
        }
    }

    // Check columns
    for (int i = 0; i < GRID_WIDTH; i++)
    {
        if (boardButtons[i].isOccupied != 0 && boardButtons[i].isOccupied == boardButtons[i + GRID_WIDTH].isOccupied && boardButtons[i + GRID_WIDTH].isOccupied == boardButtons[i + 2 * GRID_WIDTH].isOccupied)
        {
            return boardButtons[i].isOccupied;
        }
    }

    // Check diagonals
    if (boardButtons[0].isOccupied != 0 && boardButtons[0].isOccupied == boardButtons[4].isOccupied && boardButtons[4].isOccupied == boardButtons[8].isOccupied)
    {
        return boardButtons[0].isOccupied;
    }

    if (boardButtons[2].isOccupied != 0 && boardButtons[2].isOccupied == boardButtons[4].isOccupied && boardButtons[4].isOccupied == boardButtons[6].isOccupied)
    {
        return boardButtons[2].isOccupied;
    }

    return 0;
}
int checkDraw(BoardButton *boardButtons)
{
    for (int i = 0; i < GRID_WIDTH * GRID_HEIGHT; i++)
    {
        if (boardButtons[i].isOccupied == 0)
        {
            return 0;
        }
    }

    return 1;
}