#define SDL_MAIN_HANDLED

#include <cstdio>

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_mixer.h>

const char *pic001_name = "a.png";
SDL_Texture *pic001_t;

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

    // 画中间的一块绿色，虽然没用。。
    SDL_Rect rect = { 220, 140, 200, 200 };
    SDL_SetRenderDrawColor(renderer, 0, 255, 0, 100);
    SDL_RenderFillRect(renderer, &rect);

    // 在红线末端画一个滑稽。使用贴图
    SDL_Rect huaji = { x - 12, y - 12, 25, 25 };
    SDL_RenderCopy(renderer, pic001_t, NULL, &huaji);

    // 画一条红线
    SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
    SDL_RenderDrawLine(renderer, 320, 240, x, y);

    // 更新屏幕显示
    SDL_RenderPresent(renderer);

    return true;
}

// *& 是指针引用，可以在函数内部修改指针变量的值
void Init(const char *Name, SDL_Window *&window, SDL_Renderer *&renderer) {
    SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO);

    window = SDL_CreateWindow(Name,
            SDL_WINDOWPOS_UNDEFINED,
            SDL_WINDOWPOS_UNDEFINED,
            640, 480, 0);
    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);

    SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);
    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
    SDL_RenderClear(renderer);

    Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048);
}

int main() {
    SDL_Window *window = NULL;
    SDL_Renderer *renderer = NULL;

    // 初始化 SDL2 库，清屏
    Init("audio test window", window, renderer);

    // 贴图
    SDL_Surface *pic001 = NULL;
    pic001 = IMG_Load(pic001_name);
    if (pic001 == NULL)
        printf("open %s failed.\n", pic001_name);
    pic001_t = SDL_CreateTextureFromSurface(renderer, pic001);
    SDL_FreeSurface(pic001);

    // Audio
    Mix_Music *onestop;
    onestop = Mix_LoadMUS("onestop.mid");
    Mix_PlayMusic(onestop, -1);

    // 主循环
    while (Update(window, renderer))
        SDL_Delay(10);

    // 退出
    Mix_FreeMusic(onestop);
    SDL_DestroyTexture(pic001_t);
    SDL_DestroyWindow(window);
    SDL_DestroyRenderer(renderer);
    Mix_Quit();
    IMG_Quit();
    SDL_Quit();

    return 0;
}

