// Declare Library Used
#include <stdio.h>
#include <math.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_mixer.h>
#include <SDL2/SDL_ttf.h>
#include "constants.h"
#include "button.h"
#include "board_button.h"
#include "notification.h"

// Other includes and code specific to your main program
#include "easyGameLogic.h"
#include "insaneGameLogic.h"

// Run main program
int main(int argc, char *argv[])
{

  // Initialize SDL
  SDL_Init(SDL_INIT_EVERYTHING);

  if (SDL_Init(SDL_INIT_EVERYTHING) < 0)
  {
    printf("SDL initialization error: %s\n", SDL_GetError());
    // Handle the error appropriately
  }

  // Fonts for Text Rendering
  TTF_Init();

  // Initialize SDL_MIXER for music (bgm)
  Mix_Init(MIX_INIT_MP3);
  Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048); // audio settings

  // Initialize SDL_IMG for images display
  IMG_Init(IMG_INIT_PNG);
  IMG_Init(IMG_INIT_JPG);
  IMG_Init(IMG_INIT_WEBP);

  // Declare what type of TTF used
  //  TTF_Font* font = TTF_OpenFont("./resource/Blomberg.ttf", 40);

  // Create a window and declare the window properties
  SDL_Window *window = SDL_CreateWindow(
      "Grp7 TicTacToe",
      SDL_WINDOWPOS_CENTERED,
      SDL_WINDOWPOS_CENTERED,
      WINDOW_WIDTH,
      WINDOW_HEIGHT,
      SDL_WINDOW_ALLOW_HIGHDPI);

  // make renderer process and tells renderer to render the window
  SDL_Renderer *renderer = SDL_CreateRenderer(
      window,
      -1,
      SDL_RENDERER_ACCELERATED);

  // If the window cannot be loaded show an error message
  if (window == NULL)
  {
    printf("Couldn't create windows.\n%s", SDL_GetError());
    return 1;
  }

  // When doing something related to renderer, declare things
  // need before renderer presentation.

  // Making window background (Wood Texture looks nice)
  SDL_Texture *woodTexture = NULL;
  SDL_Surface *woodSurface = IMG_Load(THEME ? "./resource/img/blackwoodtexture.jpg" : "./resource/img/whitewoodtexture.jpg"); // Different theme different background
  if (woodSurface)
  {
    woodTexture = SDL_CreateTextureFromSurface(renderer, woodSurface); // This will be used later
    SDL_FreeSurface(woodSurface);                                      // Free memory
  }

  // Making BGM
  Mix_Music *bgm = Mix_LoadMUS("./resource/qtime.mp3");
  if (bgm)
  {
    Mix_PlayMusic(bgm, -1); //'-1' for loop play
  }

  // Making Menu Page Title
  Button *titleButton = createButton(renderer, (WINDOW_WIDTH - 400) / 2, 50, 400, 100, THEME ? "./resource/img/whitetitle.png" : "./resource/img/blacktitle.png");

  // Making Back Button
  Button *backButton = createButton(renderer, 10, WINDOW_HEIGHT - 90, 200, 80, THEME ? "./resource/img/whiteback.png" : "./resource/img/blackback.png");

  // Making Restart Button
  Button *restartButton = createButton(renderer, WINDOW_WIDTH - 210, WINDOW_HEIGHT - 90, 200, 80, THEME ? "./resource/img/whiterestart.png" : "./resource/img/blackrestart.png");

  // Making Buttons for Menu Page
  int buttonX = (WINDOW_WIDTH - MENU_BUTTON_WIDTH) / 2;
  int buttonY = (WINDOW_HEIGHT - MENU_BUTTON_HEIGHT + 50) / 2;
  Button *easyButton = createButton(renderer, buttonX, buttonY - 100, MENU_BUTTON_WIDTH, MENU_BUTTON_HEIGHT, "./resource/img/easy.png");
  Button *mediumButton = createButton(renderer, buttonX, buttonY, MENU_BUTTON_WIDTH, MENU_BUTTON_HEIGHT, "./resource/img/medium.png");
  Button *insaneButton = createButton(renderer, buttonX, buttonY + 100, MENU_BUTTON_WIDTH, MENU_BUTTON_HEIGHT, "./resource/img/insane.png");
  Button *twoplayersButton = createButton(renderer, buttonX, buttonY + 200, MENU_BUTTON_WIDTH, MENU_BUTTON_HEIGHT, "./resource/img/twoplayers.png");

  // Making Board Buttons for Play Page
  BoardButton gameBoardButtons[GRID_WIDTH * GRID_HEIGHT];
  initBoardButtons(renderer, gameBoardButtons);

  // Clear the renderer.
  SDL_RenderClear(renderer);
  // This is important when updating renderer, stack renderer will
  // occurred if not cleared (renderer overlap)

  // Render the wood texture as the background
  SDL_RenderCopy(renderer, woodTexture, NULL, NULL);

  // Render the title text
  // SDL_RenderCopy(renderer, titleTexture, NULL, &titleRect);
  renderButton(titleButton, renderer);

  // Render Menu Buttons
  renderButton(easyButton, renderer);
  renderButton(mediumButton, renderer);
  renderButton(insaneButton, renderer);
  renderButton(twoplayersButton, renderer);

  // Present the rendered window after everything is ready
  SDL_RenderPresent(renderer);

  // Renderer is presented and this is just the first time renderer.
  // There will be renderer in the program loop to keep update the renderer

  SDL_Event event;           // SDL event declaration
  int isRunning = 1;         // Is the window running?
  int windowState = 0;       // This will be changed between menu state (0) or game state (1)
  int windowStateMemory = 0; // This will be used to keep track if there is change in window state.
  int currentplayer = 1;     // CurrentPlayer turn
  int gameOver = 0;
  int gameLogic = 0; // To determine current game logic

  // Window event loop. this is important, without this program
  // will just create window and quit the program. Therefore while
  // loop will keep it running until I break the loop or isRunning
  // is equal to 0.

  // Program loop
  while (isRunning)
  {

    // Things to render in different page / state
    if (windowStateMemory == windowState)
    {
    }
    else if (windowStateMemory != windowState)
    {

      // Menu State
      if (windowState == 0)
      {
        SDL_RenderClear(renderer);
        SDL_RenderCopy(renderer, woodTexture, NULL, NULL);
        // SDL_RenderCopy(renderer, titleTexture, NULL, &titleRect);
        renderButton(titleButton, renderer);
        renderButton(easyButton, renderer);
        renderButton(mediumButton, renderer);
        renderButton(insaneButton, renderer);
        renderButton(twoplayersButton, renderer);
        SDL_RenderPresent(renderer);
      }
      // Game State
      else if (windowState == 1)
      {
        SDL_RenderClear(renderer);
        SDL_RenderCopy(renderer, woodTexture, NULL, NULL);
        // SDL_RenderCopy(renderer, titleTexture, NULL, &titleRect);
        renderButton(titleButton, renderer);
        renderBoardButtons(renderer, gameBoardButtons);
        renderButton(restartButton, renderer);
        renderButton(backButton, renderer);
        SDL_RenderPresent(renderer);
      }
      else
      {
        printf("Abnormal value in window State! Attention Needed ! Program Won't  work!\n");
      }

      windowStateMemory = windowState;
    }

    if (SDL_PollEvent(&event))
    {

      // Quit event
      if (event.type == SDL_QUIT)
      {
        isRunning = 0;
        break;
      }

      // Mouse event
      if (event.type == SDL_MOUSEBUTTONDOWN)
      {
        int mouseX, mouseY;
        SDL_GetMouseState(&mouseX, &mouseY);

        // Menu state Mouse Click handle
        if (windowStateMemory == 0)
        {
          if (isMouseInsideButton(easyButton, mouseX, mouseY))
          {
            windowState = 1;
            gameLogic = 0;
            getDatasetML();
            trainLR();
          }
          else if (isMouseInsideButton(mediumButton, mouseX, mouseY))
          {
            windowState = 1;
            gameLogic = 1;
          }
          else if (isMouseInsideButton(insaneButton, mouseX, mouseY))
          {
            windowState = 1;
            gameLogic = 2;
          }
          else if (isMouseInsideButton(twoplayersButton, mouseX, mouseY))
          {
            windowState = 1;
            gameLogic = 3;
          }
        }
        else if (windowStateMemory == 1)
        {
          int invalidMove = handleBoardButtonClick(gameBoardButtons, mouseX, mouseY, currentplayer, gameLogic, gameOver);
          SDL_RenderClear(renderer);
          SDL_RenderCopy(renderer, woodTexture, NULL, NULL);
          renderButton(titleButton, renderer);
          renderBoardButtons(renderer, gameBoardButtons);
          renderButton(restartButton, renderer);
          renderButton(backButton, renderer);
          SDL_RenderPresent(renderer);

          if (invalidMove == 1)
          {
            showNotification("Invalid Move", "Please choose another box");
          }
          
          if (gameOver == 0)
          {
            int isWon = checkwin(gameBoardButtons);
            if (isWon)
            {
              gameOver = 1;

              if (currentplayer == 1 && gameLogic == 3) //PVP
              {
                showNotification("Congrats", "Player X Won!");
              }
              else if (currentplayer == 2 && gameLogic ==3) //PVP
              {
                showNotification("Congrats", "Player O Won!");
              }
              else if (isWon == 1 && gameLogic == 0) //Easy
              {
                showNotification("Congrats", "You Won!");
              }
              else if (isWon == 2 && gameLogic == 0) //Easy
              {
                showNotification("Nice Try", "You Lost!");
              }
              else if (isWon == 1 && gameLogic == 1) //Medium
              {
                showNotification("Congrats", "You Won!");
              }
              else if (isWon == 2 && gameLogic == 1) //Medium
              {
                showNotification("Nice Try", "You Lost!");
              }
              else if (isWon == 1 && gameLogic == 2) //Insane
              {
                showNotification("Congrats", "You Won!");
              }
              else if (isWon == 2 && gameLogic == 2) //Insane
              {
                showNotification("Nice Try", "You Lost!");
              }
            }
            else if (checkDraw(gameBoardButtons))
            {
              gameOver = 1;
              showNotification("Good Game", "It's a Draw!");
            }
            else
            {

              if (gameLogic == 2)
              {
                // FOR INSANE
                currentplayer = 1;
              }
              else if (gameLogic == 3 && invalidMove == 0)
              {
                
                currentplayer = (currentplayer == 1) ? 2 : 1; 
              }
              else if (gameLogic == 1)
              {
                // FOR COMPUTER MEDIUM
              }
              else if (gameLogic == 0)
              {
                // FOR COMPUTER EASY
              }
            }
          }

          if (isMouseInsideButton(backButton, mouseX, mouseY))
          {
            gameOver = 0;
            for (int i = 0; i < GRID_WIDTH * GRID_HEIGHT; i++)
            {
              gameBoardButtons[i].isOccupied = 0;
            }
            currentplayer = 1;
            SDL_RenderClear(renderer);
            SDL_RenderCopy(renderer, woodTexture, NULL, NULL);
            renderButton(titleButton, renderer);
            renderBoardButtons(renderer, gameBoardButtons);
            renderButton(restartButton, renderer);
            renderButton(backButton, renderer);
            SDL_RenderPresent(renderer);

            windowState = 0;
          }
          else if (isMouseInsideButton(restartButton, mouseX, mouseY))
          {
            gameOver = 0;
            for (int i = 0; i < GRID_WIDTH * GRID_HEIGHT; i++)
            {

              gameBoardButtons[i].isOccupied = 0;
            }
            currentplayer = 1;
            SDL_RenderClear(renderer);
            SDL_RenderCopy(renderer, woodTexture, NULL, NULL);
            renderButton(titleButton, renderer);
            renderBoardButtons(renderer, gameBoardButtons);
            renderButton(restartButton, renderer);
            renderButton(backButton, renderer);
            SDL_RenderPresent(renderer);

          }
        }
      }
    }
  }

  // Destroy what have been called to prevent memory leaks
  SDL_DestroyRenderer(renderer);
  SDL_DestroyWindow(window);
  Mix_FreeMusic(bgm);
  Mix_CloseAudio();
  Mix_Quit();
  IMG_Quit();
  SDL_Quit();

  // End of Main program and terminate
  return 0;
}