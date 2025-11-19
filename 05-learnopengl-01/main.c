#include <glad/gl.h>
#include <SDL3/SDL.h>
#include <SDL3/SDL_main.h>
#include <SDL3/SDL_video.h>

const char* VertexShaderSource = 
    "#version 330 core\n"
    "layout (location = 0) in vec3 aPos;\n"
    "void main()\n"
    "{\n"
        " gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);\n"
    "}\0";

const char* FragmentShaderSource = 
    "#version 330 core\n"
    "out vec4 FragColor;\n"
    "void main()\n"
    "{\n"
        "FragColor = vec4(0.7f, 0.1f, 0.7f, 1.0f);\n"
    "}\0";

int main(int argc, char* argv[])
{
    int exit_status = 0;
    
    SDL_Window*   window     = 0;
    SDL_GLContext gl_context = 0;
    int           gl_version = 0;

    if (!SDL_Init(SDL_INIT_VIDEO)) {
        SDL_Log("SDL Initialization Failed:\n%s\n", SDL_GetError());
        exit_status = 1;
        goto exit_cleanup;
    }

    if (!SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3) ||
        !SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 3) ||
        !SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE))
    {
        SDL_Log("GL Set Attribute Failed:\n%s\n", SDL_GetError());
        exit_status = 1;
        goto exit_cleanup;
    }

    if (!(window = SDL_CreateWindow(APP_TITLE, 640, 480, SDL_WINDOW_RESIZABLE | SDL_WINDOW_OPENGL))) {
        SDL_Log("Window Creation Failed:\n%s\n", SDL_GetError());
        exit_status = 1;
        goto exit_cleanup;
    }

    if (!(gl_context = SDL_GL_CreateContext(window))) {
        SDL_Log("GL Context Creation Failed:\n%s\n", SDL_GetError());
        exit_status = 1;
        goto exit_cleanup;
    }

    if (!(gl_version = gladLoadGL((GLADloadfunc) SDL_GL_GetProcAddress))) {
        SDL_Log("Glad Loading Failed:\n");
        exit_status = 1;
        goto exit_cleanup;
    }
    glViewport(0, 0, 640, 480);
    

    // Shaders setup
    GLuint VertexShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(VertexShader, 1, &VertexShaderSource, 0);
    glCompileShader(VertexShader);

    GLuint FragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(FragmentShader, 1, &FragmentShaderSource, 0);
    glCompileShader(FragmentShader);

    SDL_Log("VertexShader:   %d\n"
            "FragmentShader: %d\n",
            VertexShader,
            FragmentShader);

    int success;
    char infoLog[512];

    glGetShaderiv(VertexShader, GL_COMPILE_STATUS, &success);
    if (!success) {
        glGetShaderInfoLog(VertexShader, 512, NULL, infoLog);
        SDL_Log("ERROR::SHADER::VERTEX::COMPILATION_FAILED\n %s\n", infoLog);
    }

    glGetShaderiv(FragmentShader, GL_COMPILE_STATUS, &success);
    if (!success) {
        glGetShaderInfoLog(FragmentShader, 512, NULL, infoLog);
        SDL_Log("ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n %s\n", infoLog);
    }

    GLuint ShaderProgram = glCreateProgram();
    glAttachShader(ShaderProgram, VertexShader);
    glAttachShader(ShaderProgram, FragmentShader);
    glLinkProgram(ShaderProgram);

    glGetProgramiv(ShaderProgram, GL_LINK_STATUS, &success);
    if (!success) {
        glGetProgramInfoLog(ShaderProgram, 512, NULL, infoLog);
        SDL_Log("ERROR::SHADER::PROGRAM::LINKING_FAILED\n%s\n", infoLog);
    }
    glDeleteShader(VertexShader);
    glDeleteShader(FragmentShader);

    float vertices[] = {
        -0.3, -0.3, 0.0,
         0.3, -0.3, 0.0,
         0.0,  0.4, 0.0
    };
    
    GLuint VBO, VAO;
    
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3*sizeof(float), 0);
    glEnableVertexAttribArray(0);
 

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);


    bool app_run = true;
    while (app_run)
    {
        // Handle events
        SDL_Event event;   
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_EVENT_QUIT)
                app_run = false;

            else if (event.type == SDL_EVENT_WINDOW_RESIZED) {
                int w, h;
                SDL_GetWindowSize(window, &w, &h);
                glViewport(0, 0, w, h);
            }
        }

        glClearColor(1, 1, 1, 1);
        glClear(GL_COLOR_BUFFER_BIT);

        glUseProgram(ShaderProgram);
        glBindVertexArray(VAO);
        glDrawArrays(GL_TRIANGLES, 0, 3);

        SDL_GL_SwapWindow(window);
        SDL_Delay(16);
    }

    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glDeleteProgram(ShaderProgram);

exit_cleanup:
    // These functions are safe to call on unitialized objects or more than once
    SDL_GL_DestroyContext(gl_context);
    SDL_DestroyWindow(window);
    SDL_Quit();

    return exit_status;
}
