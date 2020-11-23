#define SDL_MAIN_HANDLED

#include <cstdio>

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

bool Update(SDL_Window *window, SDL_Renderer *renderer) {
    static int x = 320, y = 240;

    // 用 PollEvent 拉取事件，用于检测是否需要退出程序
    SDL_Event event;
    while (SDL_PollEvent(&event)) {
        if (event.type == SDL_WINDOWEVENT_CLOSE
                || event.type == SDL_QUIT) {
            return false;
        } else if (event.type == SDL_KEYDOWN) {
            if (event.key.keysym.sym == SDLK_ESCAPE) {
                return false;
            }
        }
    }

    // GetKeyboardState 用于获取实时的键盘按键情况，可以知道哪些键是处于按下状态。据此移动人物等
    const Uint8 *state = SDL_GetKeyboardState(NULL);
    if (state[SDL_SCANCODE_LEFT]) { x -= 2; }
    if (state[SDL_SCANCODE_RIGHT]) { x += 2; }
    if (state[SDL_SCANCODE_UP]) { y -= 2; }
    if (state[SDL_SCANCODE_DOWN]) { y += 2; }

    // 处理完事件之后，更新 renderer，重绘画面
    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
    SDL_RenderClear(renderer);

    SDL_Rect rect = { 220, 140, 200, 200 };
    SDL_SetRenderDrawColor(renderer, 0, 255, 0, 100);
    SDL_RenderFillRect(renderer, &rect);

    SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
    SDL_RenderDrawLine(renderer, 320, 240, x, y);

    SDL_RenderPresent(renderer);

    return true;
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
    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
    SDL_RenderClear(renderer);

    // Main cycle
    while (Update(window, renderer))
        SDL_Delay(10);

    SDL_DestroyWindow(window);
    SDL_DestroyRenderer(renderer);
    SDL_Quit();

    return 0;
}

