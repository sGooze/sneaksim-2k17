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
        case SDL_WINDOWEVENT:
            if ((event.window.event == SDL_WINDOWEVENT_FOCUS_LOST)&&(gamefield->GetGameState() == GAMESTATE_ACTIVE))
                gamefield->SetGameState(GAMESTATE_PAUSED);
            break;
        case SDL_KEYDOWN:
            if ((event.key.keysym.scancode == SDL_SCANCODE_KP_PLUS)||(event.key.keysym.scancode == SDL_SCANCODE_KP_MINUS)){
                game_speed += (event.key.keysym.scancode == SDL_SCANCODE_KP_PLUS) ? 1 : -1;
                if (game_speed == 0) game_speed = 1;
                gamespeed_ms = (1000.0 / (float)game_speed);
            }
            if ((event.key.keysym.scancode == SDL_SCANCODE_F10))
                valid = false;
            gamefield->ParseEvent(event);
            break;
        case SDL_QUIT:
            valid = false;
            break;
        }
    }
}

void GameWrapper::StartGame(){
    if (gamefield != NULL)
        StopGame();
    gamefield = new field(64, 64);
    render.InitFieldTexture(gamefield->GetX(), gamefield->GetY());
}

#define FPS_MAX 30.0

int GameWrapper::MainLoop(){
    if (!valid)
        return -1;

    StartGame();
    auto field_obj = gamefield->GetObjectListPtr();
    auto playa_obj = gamefield->GetPlayerObjectPtr();

    frame_ms = (1000.0 / FPS_MAX); gamespeed_ms = (1000.0 / (float)game_speed);
    // Fixed time step is used since the game should be so simple that anything but microwavers should be able to run it at 30/60FPS
    while (valid){
        frame_start = SDL_GetTicks();
        PollEvents();       // Get events and send them to event handlers
        render.RenderStart();

        if ((gamefield->GetGameState() == GAMESTATE_ACTIVE)&&(frame_start - gamespeed_last > gamespeed_ms)){
            gamefield->Update(); // Update field state
            gamespeed_last = frame_start;
        }
        render.RenderField(field_obj, playa_obj);    // Render field and HUD
        render.RenderHUD(gamefield);
        if (gamefield->GetGameState() == GAMESTATE_PAUSED)
            render.RenderString("Paused!", 128, 128);
        render.RenderEnd();
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
