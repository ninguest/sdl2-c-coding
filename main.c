//Declare Library Used
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

//Run main program
int main (int argc, char* argv[]) {

  //Initialize SDL 
  SDL_Init( SDL_INIT_EVERYTHING );

  if (SDL_Init(SDL_INIT_EVERYTHING) < 0) {
    printf("SDL initialization error: %s\n", SDL_GetError());
    // Handle the error appropriately
  }

  //Fonts for Text Rendering
  TTF_Init();
  
  //Initialize SDL_MIXER for music (bgm)
  Mix_Init(MIX_INIT_MP3);
  Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048); //audio settings

  //Initialize SDL_IMG for images display
  IMG_Init(IMG_INIT_PNG);
  IMG_Init(IMG_INIT_JPG);
  IMG_Init(IMG_INIT_WEBP);

  //Declare what type of TTF used
  // TTF_Font* font = TTF_OpenFont("./resource/Blomberg.ttf", 40);

  //Create a window and declare the window properties
  SDL_Window *window = SDL_CreateWindow(
    "Grp7 TicTacToe",
    SDL_WINDOWPOS_CENTERED,
    SDL_WINDOWPOS_CENTERED,
    WINDOW_WIDTH,
    WINDOW_HEIGHT,
    SDL_WINDOW_ALLOW_HIGHDPI
  );

  //make renderer process and tells renderer to render the window
  SDL_Renderer *renderer = SDL_CreateRenderer(
    window,
    -1,
    SDL_RENDERER_ACCELERATED
  );

  //If the window cannot be loaded show an error message
  if(window == NULL){
    printf("Couldn't create windows.\n%s", SDL_GetError());
    return 1;
  }
  
  //When doing something related to renderer, declare things
  //need before renderer presentation.

  //Making window background (Wood Texture looks nice)
  SDL_Texture* woodTexture = NULL;
  SDL_Surface* woodSurface = IMG_Load(THEME ? "./resource/img/blackwoodtexture.jpg" : "./resource/img/whitewoodtexture.jpg"); //Different theme different background
  if(woodSurface){
    woodTexture = SDL_CreateTextureFromSurface(renderer, woodSurface); //This will be used later
    SDL_FreeSurface(woodSurface); //Free memory
  }

  //Making BGM
  Mix_Music* bgm = Mix_LoadMUS("./resource/qtime.mp3");
  if(bgm){
    Mix_PlayMusic(bgm, -1); //'-1' for loop play
  }

  //Making Menu Page Title 
  // SDL_Color white = {255, 255, 255, 255};
  // SDL_Color black = {0, 0, 0, 255};
  // SDL_Surface* titleSurface = TTF_RenderText_Blended(font, "Tic-Tac-Toe", (THEME ? white : black));
  // SDL_Surface* titleSurface = IMG_Load(THEME ? "./resource/img/whitetitle.png" : "./resource/img/blacktitle.png"); //Different theme different title
  // SDL_Texture* titleTexture = SDL_CreateTextureFromSurface(renderer, titleSurface);
  // SDL_FreeSurface(titleSurface); //Free Memory
  Button* titleButton = createButton(renderer, (WINDOW_WIDTH - 400) / 2, 50, 400, 100, THEME ? "./resource/img/whitetitle.png" : "./resource/img/blacktitle.png");

  //Making Back Button
  Button* backButton = createButton(renderer, 10, WINDOW_HEIGHT - 90, 200, 80, THEME ? "./resource/img/whiteback.png" : "./resource/img/blackback.png");

  //Making Restart Button
  Button* restartButton = createButton(renderer, WINDOW_WIDTH - 210, WINDOW_HEIGHT - 90, 200, 80, THEME ? "./resource/img/whiterestart.png" : "./resource/img/blackrestart.png");

  //Making rectangle (area) for rendering the title
  // SDL_Rect titleRect;
  // titleRect.x = (WINDOW_WIDTH - titleSurface->w) / 2;
  // titleRect.y = 50; // Y position
  // titleRect.w = titleSurface->w;
  // titleRect.h = titleSurface->h;

  //Making Buttons for Menu Page
  int buttonX = (WINDOW_WIDTH - MENU_BUTTON_WIDTH) / 2;
  int buttonY = (WINDOW_HEIGHT - MENU_BUTTON_HEIGHT + 50) / 2;
  Button* singlePlayerButton = createButton(renderer, buttonX, buttonY - 100, MENU_BUTTON_WIDTH, MENU_BUTTON_HEIGHT, "./resource/img/singleplayer.png");
  Button* twoPlayerButton = createButton(renderer, buttonX, buttonY, MENU_BUTTON_WIDTH, MENU_BUTTON_HEIGHT, "./resource/img/twoplayer.png");
  Button* insaneSinglePlayerButton = createButton(renderer, buttonX, buttonY + 100, MENU_BUTTON_WIDTH, MENU_BUTTON_HEIGHT, "./resource/img/insane.png");
  
  //Making Board Buttons for Play Page
  BoardButton gameBoardButtons[GRID_WIDTH * GRID_HEIGHT];
  initBoardButtons(renderer, gameBoardButtons);

  //Clear the renderer. 
  SDL_RenderClear( renderer ); 
  //This is important when updating renderer, stack renderer will 
  //occurred if not cleared (renderer overlap)

  // Render the wood texture as the background
  SDL_RenderCopy(renderer, woodTexture, NULL, NULL);

  // Render the title text
  // SDL_RenderCopy(renderer, titleTexture, NULL, &titleRect);
  renderButton(titleButton, renderer);

  //Render Menu Buttons
  renderButton(singlePlayerButton, renderer);
  renderButton(twoPlayerButton, renderer);
  renderButton(insaneSinglePlayerButton, renderer);

  //Render Board Buttons
  // renderBoardButtons(renderer, gameBoardButtons);

  //Present the rendered window after everything is ready
  SDL_RenderPresent( renderer ); 

  //Renderer is presented and this is just the first time renderer.
  //There will be renderer in the program loop to keep update the renderer

  SDL_Event event; //SDL event declaration
  int isRunning = 1; //Is the window running?
  int windowState = 0; //This will be changed between menu state (0) or game state (1)
  int windowStateMemory = 0; //This will be used to keep track if there is change in window state.

  //Window event loop. this is important, without this program
  //will just create window and quit the program. Therefore while
  //loop will keep it running until I break the loop or isRunning
  //is equal to 0.
  
  //Program loop
  while( isRunning ){

    //Things to render in different page / state
    if(windowStateMemory == windowState){
      
    }else if (windowStateMemory != windowState){
      
      //Menu State
      if(windowState == 0){
        SDL_RenderClear( renderer ); 
        SDL_RenderCopy(renderer, woodTexture, NULL, NULL);
        // SDL_RenderCopy(renderer, titleTexture, NULL, &titleRect);
        renderButton(titleButton, renderer);
        renderButton(singlePlayerButton, renderer);
        renderButton(twoPlayerButton, renderer);
        renderButton(insaneSinglePlayerButton, renderer);
        SDL_RenderPresent( renderer ); 
      }
      //Game State
      else if(windowState == 1){
        SDL_RenderClear( renderer ); 
        SDL_RenderCopy(renderer, woodTexture, NULL, NULL);
        // SDL_RenderCopy(renderer, titleTexture, NULL, &titleRect);
        renderButton(titleButton, renderer);
        renderBoardButtons(renderer, gameBoardButtons);
        renderButton(restartButton, renderer);
        renderButton(backButton, renderer);
        SDL_RenderPresent( renderer );
      }
      else {
        printf("Abnormal value in window State! Attention Needed ! Program Won't  work!\n");
      }

      windowStateMemory = windowState;
    }

    if(SDL_PollEvent(&event)){
      
      //Quit event
      if(event.type == SDL_QUIT) {
        isRunning = 0;
        break;
      }

      //Mouse event
      if (event.type == SDL_MOUSEBUTTONDOWN){
        int mouseX, mouseY;
        SDL_GetMouseState( &mouseX, &mouseY );

        //Menu state Mouse Click handle
        if(windowStateMemory == 0){
          if(isMouseInsideButton(singlePlayerButton, mouseX, mouseY)){
            windowState = 1;
            printf("Pressed Single Player\n");
          }
          else if(isMouseInsideButton(twoPlayerButton, mouseX, mouseY)){
            windowState = 1;
            printf("Pressed 2 Player\n");
          }
          else if(isMouseInsideButton(insaneSinglePlayerButton, mouseX, mouseY)){
            // windowState = 1;
            printf("Pressed Insane Single Player\nShowing Notifications\n");
            showNotification("Insane Single Player", "Coming Soon!");
          }
        } else if(windowStateMemory == 1){
          handleBoardButtonClick(gameBoardButtons, mouseX, mouseY);
          
          if(isMouseInsideButton(backButton, mouseX, mouseY)){
            windowState = 0;
            printf("Back to Menu\n");
          }
          else if(isMouseInsideButton(restartButton, mouseX, mouseY)){
            printf("Restart Game\n");
          }

        }
      }


    }
  }

  //Destroy what have been called to prevent memory leaks
  SDL_DestroyRenderer(renderer);
  SDL_DestroyWindow(window);
  Mix_FreeMusic(bgm);
  Mix_CloseAudio();
  Mix_Quit();
  IMG_Quit();
  SDL_Quit();
  
  //End of Main program and terminate
  return 0;
}