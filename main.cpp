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
    //uint16_t fps = 0, frame_last = SDL_GetTicks();
    // Fixed time step is used since the game should be so simple that anything but microwaves should be able to run it at 30/60FPS
    while (valid){
        frame_start = SDL_GetTicks();
        // Get events and send them to event handlers
        PollEvents();
        // Update field state
        // Render field and HUD
        render.Render();
        //frame_d = SDL_GetTicks() - frame_start;         // Difference in time between last and current frames
        sleep = frame_start + frame_ms - SDL_GetTicks();
        if (sleep < 0)
            sleep = 0;
        /*if (SDL_GetTicks() - frame_last > 1000){
            frame_last = SDL_GetTicks();
            title = patch::to_string(fps);
            render.WindowSetTitle(title);
            fps = 0;
        } else {fps++;}*/
        SDL_Delay(sleep);
    }
    return 0;
}

int main(int argc, char **args){
    GameWrapper game;
    return game.MainLoop();
}
