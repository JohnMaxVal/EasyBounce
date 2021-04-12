//#include <SDL.h>
#include <stdio.h>
#include <SDL.h>
#include "./constants.h"

int game_is_running = FALSE;
SDL_Window* window = NULL;
SDL_Renderer* renderer = NULL;

int last_frame_time = 0;
// keep time in ms from the last called frame

struct ball {
    float x;
    float y;
    float width;
    float height;
} Ball;

struct paddle {
    float x;
    float y;
    float width;
    float height;
} Paddle;

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

void process_input(void) {
    SDL_Event event;
    SDL_PollEvent(&event);

    switch(event.type) {
        case SDL_QUIT:
            game_is_running = FALSE;
            break;
        case SDL_KEYDOWN:
            if(event.key.keysym.sym == SDLK_ESCAPE)
                game_is_running = FALSE;
            break;
    }
}

void setup_ball(void) {
    Ball.x = 20;
    Ball.y = 20;
    Ball.width = 15;
    Ball.height = 15;
}

void setup_paddle(void) {
    Paddle.x = 20;
    Paddle.y = -20 + SCREEN_HEIGHT;
    Paddle.width = Ball.width * 5;
    Paddle.height = Ball.height;
}

void setup(void) {
    setup_ball();
    setup_paddle();
}

void update(void) {
    int time_to_wait = FRAME_TARGET_TIME - (SDL_GetTicks() - last_frame_time);

    if(time_to_wait > 0 && time_to_wait <= FRAME_TARGET_TIME) {
        SDL_Delay(time_to_wait);
    }
    //while(!SDL_TICKS_PASSED(SDL_GetTicks(), last_frame_time + FRAME_TARGET_TIME));
    // Get a delta time factor converted to secods
    float delta_time = (SDL_GetTicks() - last_frame_time) / 1000.0f;
    // Keeping fixed timestamp
    last_frame_time = SDL_GetTicks();

    Ball.x += 70 * delta_time;
    Ball.y += 50 * delta_time;
}

void draw_object(SDL_Rect rect) {
    SDL_SetRenderDrawColor(renderer, 0xff, 0xff, 0xff, 0xff);
    SDL_RenderFillRect(renderer, &rect);
}

void init_ball_rect(SDL_Rect* ball_rect) {
    ball_rect->x = (int)Ball.x;
    ball_rect->y = (int)Ball.y;
    ball_rect->w = (int)Ball.width;
    ball_rect->h = (int)Ball.height;
}

void init_paddle_rect(SDL_Rect* paddle_rect) {
    paddle_rect->x = (int)Paddle.x;
    paddle_rect->y = (int)Paddle.y;
    paddle_rect->w = (int)Paddle.width;
    paddle_rect->h = (int)Paddle.height;
}

void render(void) {
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    SDL_RenderClear(renderer);

    // Draw a rectangle
    SDL_Rect ball_rect;
    init_ball_rect(&ball_rect);
    draw_object(ball_rect);

    SDL_Rect paddle_rect;
    init_paddle_rect(&paddle_rect);
    draw_object(paddle_rect);
    // Buffers swapping
    SDL_RenderPresent(renderer);
}

void destroy_window() {
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
}

int main(int argc, char* argv[]) {
    game_is_running = initialize_window();

    setup(); // environment, objects, etc...

    while(game_is_running) {
        process_input();
        update();
        render();
    }

    destroy_window();

    return 0;
}