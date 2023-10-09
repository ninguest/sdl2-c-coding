#include "button.h"
#include <stdio.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>

Button* createButton(SDL_Renderer* renderer, int x, int y, int width, int height, const char* imagePath) {
    Button* button = (Button*)malloc(sizeof(Button));
    if (!button) {
        return NULL;
    }

    button->rect.x = x;
    button->rect.y = y;
    button->rect.w = width;
    button->rect.h = height;

    // Load the button texture
    SDL_Surface* surface = IMG_Load(imagePath);
    if (!surface) {
        free(button);
        return NULL;
    }

    button->texture = SDL_CreateTextureFromSurface(renderer, surface);
    SDL_FreeSurface(surface);

    return button;
}

int isMouseInsideButton(Button* button, int mouseX, int mouseY) {
    return (
        mouseX >= button->rect.x && 
        mouseX <= (button->rect.x + button->rect.w) &&
        mouseY >= button->rect.y && 
        mouseY <= (button->rect.y + button->rect.h)
    );
}

void renderButton(Button* button, SDL_Renderer* renderer) {
    if (button && renderer) {
        // Render the button texture (image)
        SDL_RenderCopy(renderer, button->texture, NULL, &(button->rect));
    } else {
        printf("Invalid button or renderer.\n");
    }
}

void destroyButton(Button* button) {
    SDL_DestroyTexture(button->texture);
    free(button);
}
