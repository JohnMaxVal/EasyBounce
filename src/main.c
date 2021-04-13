//#include <SDL.h>
#include <stdio.h>
#include <SDL.h>
#include <math.h>
#include "./constants.h"

int game_is_running = FALSE;
int game_is_freezed = FALSE;
SDL_Window* window = NULL;
SDL_Renderer* renderer = NULL;

int last_frame_time = 0;
// keep time in ms from the last called frame

struct GameObject {
    float x;
    float y;
    float width;
    float height;
    float x_velocity;
    float y_velocity;
} Ball, Paddle;

// struct ball {
//     float x;
//     float y;
//     float width;
//     float height;
//     float x_velocity;
//     float y_velocity;
// } Ball;

// struct paddle {
//     float x;
//     float y;
//     float width;
//     float height;
//     float x_velocity;
//     float y_velocity;
// } Paddle;

typedef struct coords {
	// Left top
	int left_top_x;
	int left_top_y;
	// Left bottom
	int left_bottom_x;
	int left_bottom_y;
	// Right top
	int right_top_x;
	int right_top_y;
	// Right Bottom
	int right_bottom_x;
	int right_bottom_y;
} Coords;

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

void setup_ball(void) {
    Ball.x = 20;
    Ball.y = 20;
    Ball.width = 15;
    Ball.height = 15;
    Ball.x_velocity = 300;
    Ball.y_velocity = 300;
}

void setup_paddle(void) {
    Paddle.x = 0;
    Paddle.y = -20 + SCREEN_HEIGHT;
    Paddle.width = Ball.width * 7;
    Paddle.height = Ball.height;
    Paddle.x_velocity = 0;
    Paddle.y_velocity = 0;
}

void freeze_objects(void) {
    game_is_freezed = TRUE;
    Ball.x_velocity = 0;
    Ball.y_velocity = 0;
    Paddle.x_velocity = 0;
    Paddle.y_velocity = 0;
}

void setup(void) {
    setup_ball();
    setup_paddle();
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
            else if(event.key.keysym.sym == SDLK_LEFT && !game_is_freezed) {
                Paddle.x_velocity = -400;
            }
            else if(event.key.keysym.sym == SDLK_RIGHT && !game_is_freezed) {
                Paddle.x_velocity = 400;
            }
            else if(event.key.keysym.sym == SDLK_SPACE) {
                game_is_freezed = FALSE;
                setup();
            }
            break;
    }
}

// void init_object_coords(Coords* ball_coords, struct GameObject game_obj) {
//     int obj_x = (int)game_obj.x;
//     int obj_y = (int)game_obj.y;
//     int obj_w = (int)game_obj.width;
//     int obj_h = (int)game_obj.height;

//     ball_coords->left_top_x = ball_coords->left_bottom_x = obj_x;
//     ball_coords->left_top_y = ball_coords->right_top_y = obj_y;
//     ball_coords->right_top_x = ball_coords->right_bottom_x = obj_x + obj_w;
//     ball_coords->left_bottom_y = ball_coords->right_bottom_y = obj_y + obj_h;
// }

void init_ball_coords(Coords* ball_coords) {
    int ball_x = (int)Ball.x;
    int ball_y = (int)Ball.y;
    int ball_w = (int)Ball.width;
    int ball_h = (int)Ball.height;

    ball_coords->left_top_x = ball_coords->left_bottom_x = ball_x;
    ball_coords->left_top_y = ball_coords->right_top_y = ball_y;
    ball_coords->right_top_x = ball_coords->right_bottom_x = ball_x + ball_w;
    ball_coords->left_bottom_y = ball_coords->right_bottom_y = ball_y + ball_h;
}

void init_paddle_coords(Coords* paddle_coords) {
    int paddle_x = (int)Paddle.x;
    int paddle_y = (int)Paddle.y;
    int paddle_w = (int)Paddle.width;
    int paddle_h = (int)Paddle.height;

    paddle_coords->left_top_x = paddle_coords->left_bottom_x = paddle_x;
    paddle_coords->left_top_y = paddle_coords->right_top_y = paddle_y;
    paddle_coords->right_top_x = paddle_coords->right_bottom_x = paddle_x + paddle_w;
    paddle_coords->left_bottom_y = paddle_coords->right_bottom_y = paddle_y + paddle_h;
}

int check_right_objects_collision(Coords* b_coords, Coords* p_coords) {
    int x_collision = (b_coords->left_bottom_x >= p_coords->right_top_x);
	int y_collision = (b_coords->left_bottom_y >= p_coords->right_top_y);
	return x_collision && y_collision;
}

int check_left_objects_collision(Coords* b_coords, Coords* p_coords) {
    int x_collision = (b_coords->right_bottom_x >= p_coords->left_top_x);
	int y_collision = (b_coords->right_bottom_y >= p_coords->left_top_y);
	return x_collision && y_collision;
}

int check_x_border_collision(Coords* b_coords) {
    return (b_coords->right_top_x >= SCREEN_WIDTH || b_coords->left_top_x <= 0);
}

int check_y_border_collision(Coords* b_coords) {
    return (b_coords->right_top_y <= 0);
}

int check_bottom_border(Coords* b_coords) {
    return (b_coords->right_bottom_y >= SCREEN_HEIGHT);
}

void collision(Coords* ball_coords, Coords* paddle_coords) {
    if(check_right_objects_collision(ball_coords, paddle_coords) || check_x_border_collision(ball_coords)) {
        Ball.x_velocity = -Ball.x_velocity;
    }
    else if(check_left_objects_collision(ball_coords, paddle_coords) || check_y_border_collision(ball_coords)) {
        Ball.y_velocity = -Ball.y_velocity;
    }

    if(paddle_coords->left_top_x <= 0) {
        Paddle.x = 0;
    }
    else if(paddle_coords->right_top_x >= SCREEN_WIDTH) {
        Paddle.x = SCREEN_WIDTH - Paddle.width;
    }

    if(check_bottom_border(ball_coords)) {
        freeze_objects();
        Ball.x_velocity = 0;
        Ball.y_velocity = 0;
    }
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

    Ball.x += Ball.x_velocity * delta_time;
    Ball.y += Ball.y_velocity * delta_time;

    Paddle.x += Paddle.x_velocity * delta_time;
    Paddle.y += Paddle.y_velocity * delta_time;

    Coords ball_coords = {0};
    //init_object_coords(&ball_coords, Ball);
    init_ball_coords(&ball_coords);
    Coords paddle_coords = {0};
    //init_object_coords(&ball_coords, Paddle);
    init_paddle_coords(&paddle_coords);

    collision(&ball_coords, &paddle_coords);
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