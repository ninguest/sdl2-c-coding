//When creating header file no need to declare the library again
//But for easy coding with library assistance is okay to include the library again
//If conflict is found just '//' all the library in header file
#ifndef BUTTON_H
#define BUTTON_H

//Declare library
#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>


//Make a structure for Button (Act as class in Javascript)
typedef struct {
  SDL_Rect rect; //In rect, there is x-position, y-position, width, height
  SDL_Texture* texture; // Texture for the button
  int isClicked; //value to determine the button is clicked
} Button;

// Function to check if a point (mouse click) is inside the button
int isMouseInsideButton(Button* button, int mouseX, int mouseY);

// Function to initialize a button
Button* createButton(SDL_Renderer* renderer, int x, int y, int width, int height, const char* imagePath);

// Function to render the button
void renderButton(Button* button, SDL_Renderer* renderer);

// Function to free button resources
void destroyButton(Button* button);

#endif