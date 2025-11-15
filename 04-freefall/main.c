#include <SDL3/SDL.h>
#include <SDL3/SDL_main.h>
#include <stdio.h>

typedef struct {
    float pos[2];
    float vel[2];
    float acc[2];
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
        .pos = { canvas_w*0.5, canvas_h*0.5 },
        .vel = { 0, 0 },
        .acc = { 0, 0 },
        .height = canvas_h*0.04,
        .width = canvas_h*0.04,
        .gravity = false
    };

    const bool* keys = SDL_GetKeyboardState(NULL);
    while (run)
    {
        // Handle events
        SDL_Event event;   
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_EVENT_QUIT)
                run = false;
        }

        /* Get key states */
        static bool enter_old = false;
        bool enter = keys[SDL_SCANCODE_RETURN];
        bool up    = keys[SDL_SCANCODE_UP];
        bool down  = keys[SDL_SCANCODE_DOWN];
        bool left  = keys[SDL_SCANCODE_LEFT];
        bool right = keys[SDL_SCANCODE_RIGHT];

        if (enter > enter_old)
            obj.gravity = !obj.gravity;
        enter_old = enter;

        float vel_from_key[2];
        if (up && !down && !obj.gravity)
            vel_from_key[1] = 500;
        else if (!up && down && !obj.gravity)
            vel_from_key[1] = -500;
        else
            vel_from_key[1] = 0;

        if (left && !right)
            vel_from_key[0] = -500;
        else if (!left && right)
            vel_from_key[0] = 500;
        else
            vel_from_key[0] = 0;

        /* Calc time */
        static SDL_Time t0 = 0;
        if (t0 == 0)
            SDL_GetCurrentTime(&t0);

        SDL_Time t1;
        SDL_GetCurrentTime(&t1);

        float delta_t = (t1 - t0) * 1e-9f; // ns -> s
        t0 = t1;

        if (obj.gravity)
            obj.acc[1] = -3000;
        else
            obj.acc[1] = 0;

        static bool gravity_old = false;
        if (gravity_old && !obj.gravity)
            obj.vel[1] = 0;
        gravity_old = obj.gravity;

        /* Update obj speed */
        obj.vel[0] += obj.acc[0] * delta_t;
        obj.vel[1] += obj.acc[1] * delta_t;

        /* Update obj position */
        float next_pos_x = obj.pos[0] + (obj.vel[0] + vel_from_key[0])*delta_t;
        float next_pos_y = obj.pos[1] + (obj.vel[1] + vel_from_key[1])*delta_t;

        if (next_pos_y < 0) {
            obj.vel[1] = 0;
            obj.acc[1] = 0;
            obj.pos[1] = 0;
        } else if (next_pos_y > canvas_h) {
            obj.pos[1] = canvas_h;
        } else {
            obj.pos[1] = next_pos_y;
        }

        obj.pos[0] = ((next_pos_x >= 0) && (next_pos_x <= canvas_w)) ? next_pos_x : obj.pos[0];
        
        /* Draw scene */
        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 0);
        SDL_RenderClear(renderer);

        SDL_FRect obj_rect = {
            .h = obj.height,
            .w = obj.width,
            .x = obj.pos[0] - obj.width*0.5,
            .y = canvas_h - (obj.pos[1] + obj.height*0.5)
        };

        if (obj.gravity)
            SDL_SetRenderDrawColor(renderer, 0xAA, 0, 0x88, 0xFF);
        else
            SDL_SetRenderDrawColor(renderer, 0, 0xAA, 0x88, 0xFF);
        SDL_RenderFillRect(renderer, &obj_rect);

        SDL_Texture* tex = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_ABGR8888, SDL_TEXTUREACCESS_STREAMING, canvas_w, canvas_h);
        SDL_SetRenderTarget(renderer, tex);
        SDL_SetRenderDrawColor(renderer, 0xFF, 0xFF, 0xFF, 0xFF);
        SDL_SetRenderScale(renderer, 3, 3);
        SDL_RenderDebugTextFormat(renderer, 5, SDL_DEBUG_TEXT_FONT_CHARACTER_SIZE*0*1.5+5, "Gravity:%s", (obj.gravity?"ON":"OFF") );
        SDL_RenderDebugTextFormat(renderer, 5, SDL_DEBUG_TEXT_FONT_CHARACTER_SIZE*1*1.5+5, "X:%d", (int)obj.pos[0]);
        SDL_RenderDebugTextFormat(renderer, 5, SDL_DEBUG_TEXT_FONT_CHARACTER_SIZE*2*1.5+5, "Y:%d", (int)obj.pos[1]);
        const char* help[] = {"Press enter to toggle gravity", "Use arrow keys to move"};
        SDL_RenderDebugText(renderer, (canvas_w/3)-(SDL_strlen(help[0])*SDL_DEBUG_TEXT_FONT_CHARACTER_SIZE)-5, SDL_DEBUG_TEXT_FONT_CHARACTER_SIZE*0*1.5+5, help[0]);
        SDL_RenderDebugText(renderer, (canvas_w/3)-(SDL_strlen(help[1])*SDL_DEBUG_TEXT_FONT_CHARACTER_SIZE)-5, SDL_DEBUG_TEXT_FONT_CHARACTER_SIZE*1*1.5+5, help[1]);

        SDL_SetRenderScale(renderer, 1, 1);
        SDL_SetRenderTarget(renderer, NULL);
        SDL_RenderTexture(renderer, tex, NULL, NULL);
        SDL_DestroyTexture(tex);

        SDL_RenderPresent(renderer);
        SDL_Delay(16);
    }
    
    /* Cleanup */
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(win);
    SDL_Quit();
}
