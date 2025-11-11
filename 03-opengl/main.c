#include <glad/gl.h>
#include <SDL3/SDL.h>
#include <SDL3/SDL_main.h>
#include <SDL3/SDL_video.h>

int main(int argc, char* argv[])
{
    SDL_Init(SDL_INIT_VIDEO);

    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 3);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);

    SDL_Window*   win;
    win = SDL_CreateWindow(APP_TITLE,
                           640, 480,
                           SDL_WINDOW_RESIZABLE | SDL_WINDOW_OPENGL);

    SDL_GLContext gl_context = SDL_GL_CreateContext(win);

    int version = gladLoadGL((GLADloadfunc) SDL_GL_GetProcAddress);
    // printf("GL %d.%d\n", GLAD_VERSION_MAJOR(version), GLAD_VERSION_MINOR(version));

    bool run = win != 0;

    while (run)
    {
        // Handle events
        SDL_Event event;   
        while (SDL_PollEvent(&event))
            if (event.type == SDL_EVENT_QUIT)
                run = false;

        static float t = 0;
        glClearColor(SDL_fabsf(SDL_sinf(3*t))*0.75, 0, 0, 1);
        glClear(GL_COLOR_BUFFER_BIT);
        SDL_GL_SwapWindow(win);

        SDL_Delay(32);
        t += 0.032;
    }
    
    SDL_GL_DestroyContext(gl_context);
    SDL_DestroyWindow(win);
    SDL_Quit();
}
