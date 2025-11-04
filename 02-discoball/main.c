#include <SDL3/SDL.h>
#include <SDL3/SDL_main.h>

int main(int argc, char* argv[])
{
    SDL_Init(SDL_INIT_VIDEO);

    SDL_Window*   win;
    SDL_Renderer* renderer;
    bool run = SDL_CreateWindowAndRenderer(APP_TITLE, 640, 480, SDL_WINDOW_RESIZABLE, &win, &renderer);

    while (run)
    {
        // Handle events
        SDL_Event event;   
        while (SDL_PollEvent(&event))
            if (event.type == SDL_EVENT_QUIT)
                run = false;

        // Drawing
        int win_w, win_h;
        SDL_GetWindowSize(win, &win_w, &win_h);

        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
        SDL_RenderClear(renderer);
        SDL_SetRenderDrawColor(renderer, SDL_rand(256), SDL_rand(256), SDL_rand(256), 255);
        
        float side_len = (win_h < win_w ? win_h : win_w)*0.2; // 20% of narrower side
        
        SDL_FRect rect = {(win_w/2)-(side_len/2), (win_h/2)-(side_len/2), side_len, side_len}; // x, y, w, h
        SDL_RenderFillRect(renderer, &rect);

        SDL_SetRenderDrawColor(renderer, SDL_rand(256), SDL_rand(256), SDL_rand(256), 255);
        rect.x += side_len;
        SDL_RenderFillRect(renderer, &rect);

        SDL_SetRenderDrawColor(renderer, SDL_rand(256), SDL_rand(256), SDL_rand(256), 255);
        rect.x -= 2*side_len;
        SDL_RenderFillRect(renderer, &rect);

        SDL_SetRenderDrawColor(renderer, SDL_rand(256), SDL_rand(256), SDL_rand(256), 255);
        rect.x += side_len;
        rect.y += side_len;
        SDL_RenderFillRect(renderer, &rect);

        SDL_SetRenderDrawColor(renderer, SDL_rand(256), SDL_rand(256), SDL_rand(256), 255);
        rect.y -= 2*side_len;
        SDL_RenderFillRect(renderer, &rect);

        SDL_RenderPresent(renderer);
        SDL_Delay(100);
    }
    
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(win);
    SDL_Quit();
}
