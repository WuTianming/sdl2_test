#define SDL_MAIN_HANDLED

#include <cstdio>
#include <vector>

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

using namespace std;

const char *pic001_name = "a.png";
SDL_Texture *pic001_t;

Uint32 lastTime = 0;

struct Bullet {
    double velX, velY;      // per tick
    int posX, posY;
    int radius;
};
// vector<Bullet> bullets;
Bullet bullet;
// int bId = 0;

bool Update(SDL_Window *window, SDL_Renderer *renderer) {
    static int x = 320, y = 240;
    static int frames = 0;

    // 用 PollEvent 拉取事件，用于检测是否需要退出程序
    SDL_Event event;
    while (SDL_PollEvent(&event)) {
        if (event.type == SDL_WINDOWEVENT_CLOSE
                || event.type == SDL_QUIT) {
            return false;
        } else if (event.type == SDL_KEYDOWN && event.key.repeat == 0) {
            if (event.key.keysym.sym == SDLK_ESCAPE) {
                return false;
            }
            /* if (event.key.keysym.sym == SDLK_f) {
                bullets.push_back((Bullet){ 0.00, 0.00, x, y, 10 });
            } */
            /* if (event.key.keysym.sym == SDLK_w) { bullet.velY -= 2; }
            if (event.key.keysym.sym == SDLK_a) { bullet.velX -= 2; }
            if (event.key.keysym.sym == SDLK_s) { bullet.velY += 2; }
            if (event.key.keysym.sym == SDLK_d) { bullet.velX += 2; } */
        }
    }

    // GetKeyboardState 用于获取实时的键盘按键情况，可以知道哪些键是处于按下状态。据此移动人物等
    const Uint8 *state = SDL_GetKeyboardState(NULL);
    if (state[SDL_SCANCODE_LEFT]) { x -= 2; }
    if (state[SDL_SCANCODE_RIGHT]) { x += 2; }
    if (state[SDL_SCANCODE_UP]) { y -= 2; }
    if (state[SDL_SCANCODE_DOWN]) { y += 2; }
    if (state[SDL_SCANCODE_W]) { bullet.velY -= 10; }
    if (state[SDL_SCANCODE_A]) { bullet.velX -= 10; }
    if (state[SDL_SCANCODE_S]) { bullet.velY += 10; }
    if (state[SDL_SCANCODE_D]) { bullet.velX += 10; }

    Uint32 nowTime = SDL_GetTicks();
    bullet.posX += bullet.velX * (nowTime - lastTime) / 1000.00;
    if (bullet.posX < 0) bullet.posX += 640;
    else if (bullet.posX >= 640) bullet.posX -= 640;
    bullet.posY += bullet.velY * (nowTime - lastTime) / 1000.00;
    if (bullet.posY < 0) bullet.posY += 480;
    else if (bullet.posY >= 480) bullet.posY -= 480;

    // 处理完事件之后，更新 renderer，重绘画面
    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
    SDL_RenderClear(renderer);

    // draw bullet
    SDL_Rect Bul = { bullet.posX - bullet.radius,
                     bullet.posY - bullet.radius,
                     2 * bullet.radius, 2 * bullet.radius };
    SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
    SDL_RenderFillRect(renderer, &Bul);

    // 画中间的一块绿色，虽然没用。。
    SDL_Rect rect = { 220, 140, 200, 200 };
    SDL_SetRenderDrawColor(renderer, 0, 255, 0, 50);
    SDL_RenderFillRect(renderer, &rect);

    // 在红线末端画一个滑稽。使用贴图
    SDL_Rect huaji = { x - 15, y - 12, 30, 30 };
    SDL_RenderCopy(renderer, pic001_t, NULL, &huaji);

    // 画一条红线
    SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
    SDL_RenderDrawLine(renderer, 320, 240, x, y);

    // 更新屏幕显示
    SDL_RenderPresent(renderer); ++frames;
    printf("frame = %d, tick = %d, delta time = %d, fps = %lf\n",
            frames, nowTime, nowTime - lastTime, 1000.00 * frames / nowTime);
    lastTime = nowTime;

    return true;
}

// *& 是指针引用，可以在函数内部修改指针变量的值
void Init(const char *Name, SDL_Window *&window, SDL_Renderer *&renderer) {
    SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO | SDL_INIT_TIMER);

    window = SDL_CreateWindow(Name,
            SDL_WINDOWPOS_UNDEFINED,
            SDL_WINDOWPOS_UNDEFINED,
            640, 480, 0);
    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);

    SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);
    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
    SDL_RenderClear(renderer);
}

int main() {
    bullet.velX = bullet.velY = 0;
    bullet.posX = 320; bullet.posY = 240;
    bullet.radius = 20;

    SDL_Window *window = NULL;
    SDL_Renderer *renderer = NULL;

    // 初始化 SDL2 库，清屏
    Init("Game window", window, renderer);

    // 测试 贴图 功能
    // SDL_Surface 用于存储图片，SDL_Texture 则将图片处理成硬件格式来存储，后者速度较快。先使用前者来加载图片，然后将前者转换成后者，并删除(Free)前者
    SDL_Surface *pic001 = NULL;
    pic001 = IMG_Load(pic001_name);
    if (pic001 == NULL)
        printf("open %s failed.\n", pic001_name);
    pic001_t = SDL_CreateTextureFromSurface(renderer, pic001);
    SDL_FreeSurface(pic001);

    // 主循环
    while (Update(window, renderer));

    // 退出
    SDL_DestroyTexture(pic001_t);
    SDL_DestroyWindow(window);
    SDL_DestroyRenderer(renderer);
    SDL_Quit();

    return 0;
}

