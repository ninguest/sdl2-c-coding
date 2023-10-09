//Declare Library Used
#include <stdio.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include "./constants.h"

//Make a structure for Button (Act as class in Javascript)
typedef struct {
  SDL_Rect rect; //In rect, there is x-position, y-position, width, height
  SDL_Color color; //Button background color
  int isClicked; //value to determine the button is clicked
} Button;

//Function to check whether the input is the area of a button
int isHovered(Button button,  int x, int y){
  return (
    x >= button.rect.x && 
    x <= button.rect.x + button.rect.w && 
    y >= button.rect.y && 
    y <= button.rect.y + button.rect.h
  );
}

//Run main program
int main (int argc, char* argv[]) {

  //Initialize SDL 
  SDL_Init( SDL_INIT_EVERYTHING );

  //Load a image into SDL
  SDL_Surface* imageSurface = IMG_Load("./img/kboom.gif");
  SDL_Surface* hoverImageSurface = IMG_Load("./img/khmm.gif");

  //If the image is not loaded show an error message
  if (!imageSurface || !hoverImageSurface) {
    printf("Failed to load images: %s", IMG_GetError());
    return 1;
  }

  //Create a window and declare the window properties
  SDL_Window *window = SDL_CreateWindow(
    "Hello Hardcore",
    SDL_WINDOWPOS_CENTERED,
    SDL_WINDOWPOS_CENTERED,
    WINDOW_WIDTH,
    WINDOW_HEIGHT,
    SDL_WINDOW_ALLOW_HIGHDPI
  );

  //If the window cannot be loaded show an error message
  if(window == NULL){
    printf("Couldn't create windows.\n%s", SDL_GetError());
    return 1;
  }

  //make renderer process and tells renderer to render the window
  SDL_Renderer *renderer = SDL_CreateRenderer(
    window,
    -1,
    SDL_RENDERER_ACCELERATED
  );

  //Declare button with Button Structure
  Button button;

  //Button info
  button.rect.x = 100;
  button.rect.y = 100;
  button.rect.w = 100;
  button.rect.h = 100;
  button.color = (SDL_Color){255, 255, 255, 0};
  button.isClicked = 0;

  //Convert image rendered into texture for rendering
  SDL_Texture* hoverTexture = SDL_CreateTextureFromSurface(renderer, imageSurface);
  SDL_Texture* imageTexture = SDL_CreateTextureFromSurface(renderer, hoverImageSurface);

  // Free the surface as it's no longer needed. Prevent memory leaks.
  SDL_FreeSurface(imageSurface); 
  SDL_FreeSurface(hoverImageSurface);

  //Change the background color of the window
  SDL_SetRenderDrawColor( renderer, 140, 0, 100, 255 ); //Background color

  //Clear the screen
  SDL_RenderClear( renderer );

  //Draw the button
  // SDL_SetRenderDrawColor(renderer, button.color.r, button.color.g, button.color.b, button.color.a);
  SDL_RenderFillRect( renderer, &button.rect );

  //Image Render
  SDL_RenderCopy(renderer, imageTexture, NULL, &button.rect); // Render the whole image

  //Present the rendered window after everything is ready
  SDL_RenderPresent( renderer ); // End of Background color 

  //Declare event for keeping track of the window event
  SDL_Event event;
  int buttonHovered = 0;

  //while the window if running 
  while( 1 ){


    if(SDL_PollEvent(&event)){

      //QUIT Event
      if(SDL_QUIT == event.type) { break; }

      //Mouse Events within window
      if(SDL_MOUSEMOTION == event.type){
        int x, y;
        SDL_GetMouseState( &x, &y );

        if(isHovered(button, x, y)){
          buttonHovered = 1;
          SDL_RenderClear( renderer );
          SDL_RenderCopy(renderer, hoverTexture, NULL, &button.rect);
          SDL_RenderPresent( renderer );
        }
        else {
          buttonHovered = 0;
          SDL_RenderClear( renderer );
          SDL_RenderCopy(renderer, imageTexture, NULL, &button.rect);
          SDL_RenderPresent( renderer );
        }

        // printf("X: %d, Y: %d\n", x, y);
      }

      //Mouse Button Events
      if(SDL_MOUSEBUTTONDOWN == event.type){

        //Mouse Left Click
        if(SDL_BUTTON_LEFT == event.button.button ){
          
          //Whenever Mouse Left Click
          printf("Left mouse button is down\n");
          
          int x, y;
          SDL_GetMouseState( &x, &y );

          //When specific area is clicked
          if(isHovered(button, x, y)){
            button.isClicked = 1;
            printf("Clicked a button called button\n");
          }
        }

        //Mouse Right Click
        if(SDL_BUTTON_RIGHT == event.button.button){
          printf("Right mouse button is down\n");
        }

        //Mouse Middle Scroll Click
        if(SDL_BUTTON_MIDDLE == event.button.button){
          printf("Middle mouse button is down\n");
        }
      }

      //Scrolling Event
      if( SDL_MOUSEWHEEL == event.type){

        //Up scroll
        if(event.wheel.y > 0){
          printf("Mouse wheel is scrolling up");
        }

        //Down scroll
        else if(event.wheel.y < 0){
          printf("Mouse wheel is scrolling down");
        }

        //Right scroll
        if(event.wheel.x > 0){
          printf("Mouse wheel is scrolling right");
        }

        //Left scroll
        else if(event.wheel.x > 0){
          printf("Mouse wheel is scrolling left");
        }
      }

      //Keyboard Press Events
      if(SDL_KEYDOWN == event.type) {
        //printf("Key pressed");

        //When 'A' button is pressed
        if(SDLK_a == event.key.keysym.sym) {printf("A is Down\n");}
      }

      //Keyboard Release Events
      else if(SDL_KEYUP == event.type) {
        // printf("Key released");

        //When 'A' button is released
        if(SDLK_a == event.key.keysym.sym) {printf("A is Up\n");}
      }
    }

  }

  if(buttonHovered == TRUE){
    printf("Hovered button");
  } else {
    printf("Unhovered button");
  }

  //Destroy what have been called to prevent memory leaks
  SDL_DestroyTexture(imageTexture);
  SDL_DestroyRenderer(renderer);
  SDL_DestroyWindow(window);
  IMG_Quit();
  SDL_Quit();
  
  //End of Main program and terminate
  return 0;
}