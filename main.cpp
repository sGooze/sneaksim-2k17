#include "inc/sdl_app.hpp"

using namespace Sneke_SM;

bool GameWrapper::InitSDL(){
    if (SDL_Init(SDL_INIT_VIDEO)){
        return false;
    }
    return true;
}

GameWrapper::GameWrapper(){
    try {
        valid = InitSDL();
        render.WindowSetWidth(800);
        render.WindowSetHeight(480);
        render.Reset(true);
    }
    catch (std::runtime_error& rt){
        std::cout << "Runtime exception caught during initialization:\n " << rt.what();
        valid = false;
    }
}

GameWrapper::~GameWrapper(){
    std::cout << "done\n";
    SDL_Quit();
}

void GameWrapper::PollEvents(){
    static SDL_Event event;
    while (SDL_PollEvent(&event)){
        switch (event.type){
        case SDL_QUIT:
            valid = false;
            break;
        }
    }
}

#define FPS_MAX 30.0

int GameWrapper::MainLoop(){
    if (!valid)
        return -1;
    field gamefield(64, 64);
    render.InitFieldTexture(gamefield.GetX(), gamefield.GetY());

    uint32_t frame_start;
     int16_t sleep;
    uint16_t frame_ms = (1000.0 / FPS_MAX);
    // Fixed time step is used since the game should be so simple that anything but microwaves should be able to run it at 30/60FPS
    while (valid){
        frame_start = SDL_GetTicks();
        PollEvents();       // Get events and send them to event handlers
                            // Update field state
        render.Render();    // Render field and HUD
        // Pointer to object list is passed to the renderer, who then uses a set of functions to render objects to the field texture
        // RenderCommandList
        sleep = frame_start + frame_ms - SDL_GetTicks();
        if (sleep < 0)
            sleep = 0;
        SDL_Delay(sleep);
    }
    return 0;
}

int main(int argc, char **args){
    GameWrapper game;
    return game.MainLoop();
}
