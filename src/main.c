//#include <SDL.h>
#include <stdio.h>
#include <SDL.h>
#include "./constants.h"

SDL_Window* window = NULL;
SDL_Renderer* renderer = NULL;

int initialize_window(void) {
    if(SDL_Init(SDL_INIT_EVERYTHING) != 0) {
        fprintf(stderr, "Error intializing SDL.\n");
        return FALSE;
    }
    window = SDL_CreateWindow(
        NULL,
        SDL_WINDOWPOS_CENTERED,
        SDL_WINDOWPOS_CENTERED,
        SCREEN_WIDTH,
        SCREEN_HEIGHT,
        SDL_WINDOW_BORDERLESS
    );
    if(window == NULL) {
        fprintf(stderr, "Error creating SDL window.\n");
        return FALSE;
    }
    renderer = SDL_CreateRenderer(window, -1, 0);
    if(renderer == NULL) {
        fprintf(stderr, "Error creating SDL Renderer.\n");
        return FALSE;
    }
    return TRUE;
}

int main(int argc, char* argv[]) {
    initialize_window();
    return 0;
}