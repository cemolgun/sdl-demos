#include <SDL3/SDL.h>
#include <SDL3/SDL_main.h>
#include <stdio.h>

typedef struct {
    float pos_x;
    float pos_y;
    float vel_x;
    float vel_y;
    float acc_x;
    float acc_y;
    float height;
    float width;
    float mass;
} Object;

int main(int argc, char* argv[])
{
    SDL_Init(SDL_INIT_VIDEO);

    SDL_Window*   win;
    SDL_Renderer* renderer;
    bool run = SDL_CreateWindowAndRenderer(APP_TITLE, 640, 480, SDL_WINDOW_RESIZABLE, &win, &renderer);

    const float canvas_w = 1600;
    const float canvas_h = 900;

    Object obj = {
        .pos_x = canvas_w*0.5,
        .pos_y = canvas_h*0.5,
        .vel_x = 0,
        .vel_y = 0,
        .acc_x = 0,
        .acc_y = 0,
        .height = canvas_h*0.05,
        .width = canvas_h*0.05,
        .mass = 10 
    };

    while (run)
    {
        // Handle events
        SDL_Event event;   
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_EVENT_QUIT)
                run = false;

            if (event.type == SDL_EVENT_KEY_DOWN) {

                obj.vel_x = 0;
                obj.vel_y = 0;
                obj.acc_x = 0;
                obj.acc_y = 0;

                switch (event.key.key) {
                case SDLK_UP:
                    obj.pos_y += obj.height;
                    break;
                case SDLK_DOWN:
                    obj.pos_y -= obj.height;
                    break;
                case SDLK_RIGHT:
                    obj.pos_x += obj.width;
                    break;
                case SDLK_LEFT:
                    obj.pos_x -= obj.width;
                    break;
                case SDLK_RETURN:
                    break;
                }
            }
        }

        SDL_SetRenderLogicalPresentation(renderer, canvas_w, canvas_h, SDL_LOGICAL_PRESENTATION_STRETCH);

        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 0);
        SDL_RenderClear(renderer);

        int win_w, win_h;
        SDL_GetWindowSize(win, &win_w, &win_h);

        SDL_FRect obj_rect = {
            .h = obj.height,
            .w = obj.width,
            .x = obj.pos_x - obj.width*0.5,
            .y = canvas_h - (obj.pos_y + obj.height*0.5)
        };

        SDL_SetRenderDrawColor(renderer, 0xAA, 0, 0x88, 0);
        SDL_RenderFillRect(renderer, &obj_rect);


        SDL_RenderPresent(renderer);
        SDL_Delay(16);
    }
    
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(win);
    SDL_Quit();
}
