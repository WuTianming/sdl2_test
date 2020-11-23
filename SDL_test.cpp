#define SDL_MAIN_HANDLED

#include <cstdio>

#include <SDL2/SDL.h>

bool Update(SDL_Window *window, SDL_Renderer *renderer) {
    int done = 0;
    static int x = 320, y = 240;

    SDL_Event event;
    while (SDL_PollEvent(&event)) {
        if (event.type == SDL_WINDOWEVENT_CLOSE
                || event.type == SDL_QUIT) {
            done = 1; return done;
        } else if (event.type == SDL_KEYDOWN) {
            if (event.key.keysym.sym == SDLK_ESCAPE) {
                done = 1; return done;
            }
        }
    }

    const Uint8 *state = SDL_GetKeyboardState(NULL);
    if (state[SDL_SCANCODE_LEFT]) { x -= 2; }
    if (state[SDL_SCANCODE_RIGHT]) { x += 2; }
    if (state[SDL_SCANCODE_UP]) { y -= 2; }
    if (state[SDL_SCANCODE_DOWN]) { y += 2; }

    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
    SDL_RenderClear(renderer);

    SDL_Rect rect = { 220, 140, 200, 200 };
    SDL_SetRenderDrawColor(renderer, 0, 255, 0, 100);
    SDL_RenderFillRect(renderer, &rect);

    SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
    SDL_RenderDrawLine(renderer, 320, 240, x, y);

    SDL_RenderPresent(renderer);
    return done;
}

int main() {
    SDL_Window *window = NULL;
    SDL_Renderer *renderer;

    SDL_Init(SDL_INIT_VIDEO);

    window = SDL_CreateWindow("test window 1",
            SDL_WINDOWPOS_UNDEFINED,
            SDL_WINDOWPOS_UNDEFINED,
            640, 480, 0);
    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);

    SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    SDL_RenderClear(renderer);

    int done = 0;
    SDL_Event event;
    while (!done) {
        SDL_Delay(10);
        done = Update(window, renderer);
    }

    SDL_DestroyWindow(window);
    SDL_DestroyRenderer(renderer);
    SDL_Quit();

    return 0;
}

