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
        render.WindowSetWidth(640);
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
        case SDL_KEYDOWN:
            break;
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
    auto field_obj = gamefield.GetObjectListPtr();
    auto playa_obj = gamefield.GetPlayerObjectPtr();

    uint32_t frame_start, gamespeed_last = 0;
     int16_t sleep;
    uint16_t frame_ms = (1000.0 / FPS_MAX), gamespeed_ms = (1000.0 / (float)game_speed);
    // Fixed time step is used since the game should be so simple that anything but microwaves should be able to run it at 30/60FPS
    while (valid){
        frame_start = SDL_GetTicks();
        //PollEvents();       // Get events and send them to event handlers

        // TODO: move away. Events should be passed to renderer, field and menu controllers
        static SDL_Event event;
        while (SDL_PollEvent(&event)){
            switch (event.type){
            case SDL_KEYDOWN:
                if ((event.key.keysym.scancode == SDL_SCANCODE_KP_PLUS)||(event.key.keysym.scancode == SDL_SCANCODE_KP_MINUS)){
                    game_speed += (event.key.keysym.scancode == SDL_SCANCODE_KP_PLUS) ? 1 : -1;
                    if (game_speed == 0) game_speed = 1;
                    gamespeed_ms = (1000.0 / (float)game_speed);
                }
                gamefield.ParseEvent(event);
                break;
            case SDL_QUIT:
                valid = false;
                break;
            }
        }

        if (frame_start - gamespeed_last > gamespeed_ms){
            gamefield.Update(); // Update field state
            gamespeed_last = frame_start;
        }

        render.Render(field_obj, playa_obj);    // Render field and HUD
        // Pointer to object list is passed to the renderer, who then uses a set of functions to render objects to the field texture
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
