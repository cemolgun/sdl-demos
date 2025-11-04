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
        SDL_Event event;   
        while (SDL_PollEvent(&event))
            if (event.type == SDL_EVENT_QUIT)
                run = false;

        SDL_RenderPresent(renderer);
        SDL_Delay(16);
    }
    
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(win);
    SDL_Quit();
}
