#define SDL_MAIN_HANDLED

#include <cstdio>

#include <SDL2/SDL.h>

int main() {
    SDL_Window *window = NULL;
    SDL_Renderer *renderer;
    
    window = SDL_CreateWindow("test window 1",
                               SDL_WINDOWPOS_UNDEFINED,
                               SDL_WINDOWPOS_UNDEFINED,
                               640, 480, 0);
    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);

    SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    SDL_RenderClear(renderer);

    int x = 320, y = 240;
    int done = 0;
    SDL_Event event;
    while (!done) {
        while (SDL_PollEvent(&event)) {
            switch (event.type) {
                case SDL_WINDOWEVENT_CLOSE:
                    if (window) {
                        SDL_DestroyWindow(window);
                        window = NULL;
                    }
                    break;
                case SDL_KEYDOWN:
                    switch (event.key.keysym.sym) {
                        case SDLK_ESCAPE:
                            done = 1;
                            break;
                        case SDLK_UP: y -= 20; break;
                        case SDLK_DOWN: y += 20; break;
                        case SDLK_LEFT: x -= 20; break;
                        case SDLK_RIGHT: x += 20; break;
                    }
                    break;
                case SDL_QUIT:
                    done = 1;
                    break;
            }
        }

        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
        SDL_RenderClear(renderer);

        SDL_Rect rect = { 220, 140, 200, 200 };
        SDL_SetRenderDrawColor(renderer, 0, 255, 0, 100);
        SDL_RenderFillRect(renderer, &rect);

        SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
        SDL_RenderDrawLine(renderer, 320, 240, x, y);

        SDL_RenderPresent(renderer);
        SDL_Delay(50);
    }

    SDL_DestroyWindow(window);
    SDL_DestroyRenderer(renderer);
    SDL_Quit();

    return 0;
}

