#include <SDL3/SDL.h>
#include <SDL3/SDL_main.h>
#include <stdio.h>

typedef struct {
    float pos_x;
    float pos_y;
    float vel_y;
    float height;
    float width;
    bool  gravity;
} Object;

int main(int argc, char* argv[])
{
    SDL_Init(SDL_INIT_VIDEO);

    SDL_Window*   win;
    SDL_Renderer* renderer;
    
    const float canvas_w = 1600;
    const float canvas_h = 900;
    
    bool run = SDL_CreateWindowAndRenderer(APP_TITLE, 640, 360, SDL_WINDOW_RESIZABLE, &win, &renderer);
    SDL_SetRenderLogicalPresentation(renderer, canvas_w, canvas_h, SDL_LOGICAL_PRESENTATION_STRETCH);
    SDL_SetWindowAspectRatio(win, canvas_w/canvas_h, canvas_w/canvas_h);


    Object obj = {
        .pos_x = canvas_w*0.5,
        .pos_y = canvas_h*0.5,
        .vel_y = 0,
        .height = canvas_h*0.04,
        .width = canvas_h*0.04,
        .gravity = false
    };

    while (run)
    {
        // Handle events
        SDL_Event event;   
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_EVENT_QUIT)
                run = false;

            if (event.type == SDL_EVENT_KEY_DOWN) {

                switch (event.key.key) {
                case SDLK_UP:
                    if (!obj.gravity)
                        obj.pos_y = (obj.pos_y + obj.height) < canvas_h ? obj.pos_y + obj.height : canvas_h;
                    break;
                case SDLK_DOWN:
                    obj.pos_y = (obj.pos_y - obj.height) > 0 ? obj.pos_y - obj.height : 0;
                    break;
                case SDLK_RIGHT:
                    obj.pos_x = (obj.pos_x + obj.width) < canvas_w ? obj.pos_x + obj.width : canvas_w;
                    break;
                case SDLK_LEFT:
                    obj.pos_x = (obj.pos_x - obj.width) > 0 ? obj.pos_x - obj.width : 0;
                    break;
                case SDLK_RETURN:
                    obj.gravity = !obj.gravity;
                    break;
                }
            }
        }

        static SDL_Time t0 = 0;
        if (t0 == 0)
            SDL_GetCurrentTime(&t0);

        SDL_Time t1;
        SDL_GetCurrentTime(&t1);

        float delta_t = (t1 - t0) * 1e-9f; // ns -> s
        t0 = t1;

        obj.pos_y = (obj.pos_y + obj.vel_y*delta_t) > 0 ? (obj.pos_y + obj.vel_y*delta_t) : 0;

        if (obj.gravity && obj.pos_y != 0)
            obj.vel_y += -9.81*200 * delta_t;
        else
            obj.vel_y = 0;

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

        if (obj.gravity)
            SDL_SetRenderDrawColor(renderer, 0xAA, 0, 0x88, 0xFF);
        else
            SDL_SetRenderDrawColor(renderer, 0, 0xAA, 0x88, 0xFF);
        SDL_RenderFillRect(renderer, &obj_rect);
            
        SDL_SetRenderDrawColor(renderer, 0xFF, 0xFF, 0xFF, 0xFF);
        SDL_RenderDebugText(renderer, 0, 0, "Press enter to toggle gravity");

        SDL_RenderPresent(renderer);
        SDL_Delay(16);
    }
    
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(win);
    SDL_Quit();
}
