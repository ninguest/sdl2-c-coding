#ifndef NOTIFICATION_H
#define NOTIFICATION_H

#include <SDL2/SDL.h>

void showNotification(const char* title, const char* message){
    // Initialize SDL
    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        SDL_Log("SDL initialization error: %s", SDL_GetError());
    }

    // Display a simple message box
    SDL_MessageBoxButtonData buttons[] = {
        { SDL_MESSAGEBOX_BUTTON_RETURNKEY_DEFAULT, 0, "OK" },
    };
    SDL_MessageBoxData messageboxdata = {
        SDL_MESSAGEBOX_INFORMATION,
        NULL, // No parent window
        title,
        message,
        SDL_arraysize(buttons),
        buttons,
        NULL // Default color scheme
    };

    int buttonid;
    if (SDL_ShowMessageBox(&messageboxdata, &buttonid) < 0) {
        SDL_Log("Error displaying message box: %s", SDL_GetError());
    }
}

#endif