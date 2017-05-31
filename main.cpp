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
        frame_ms = (1000.0 / FPS_MAX);
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
            /*if ((event.window.event == SDL_WINDOWEVENT_FOCUS_LOST)&&(gamefield->GetGameState() == GAMESTATE_ACTIVE))
                gamefield->SetGameState(GAMESTATE_PAUSED);*/
            break;
        case SDL_KEYDOWN:
            if ((event.key.keysym.scancode == SDL_SCANCODE_KP_PLUS)||(event.key.keysym.scancode == SDL_SCANCODE_KP_MINUS)){
                game_speed += (event.key.keysym.scancode == SDL_SCANCODE_KP_PLUS) ? 1 : -1;
                if (game_speed == 0) game_speed = 1;
                gamespeed_ms = (1000.0 / (float)game_speed);
            }
            if ((event.key.keysym.scancode == SDL_SCANCODE_F10))
                valid = false;
            if ((event.key.keysym.scancode == SDL_SCANCODE_R)&&(gamefield->GetGameState() == GAMESTATE_FINISHED)){
                StartGame();
                break;
            }
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
    gamefield = new field(32, 32, temp_walls);
    render.InitFieldTexture(gamefield->GetX(), gamefield->GetY());
    field_obj = gamefield->GetObjectListPtr()->getAllObjects();
    playa_obj = gamefield->GetPlayerObjectPtr();
    gamespeed_ms = (1000.0 / (float)game_speed);
}

void GameWrapper::StopGame(){
    delete gamefield;
    field_obj = NULL;
    playa_obj = NULL;
}

int GameWrapper::MainMenuLoop(){
    // PLACEHOLDER!
    static SDL_Event event;
    while (valid){
        StartFrame();

        while (SDL_PollEvent(&event)){
            switch (event.type){
            case SDL_KEYDOWN:
                if ((event.key.keysym.scancode == SDL_SCANCODE_KP_PLUS)||(event.key.keysym.scancode == SDL_SCANCODE_KP_MINUS)){
                    game_speed += (event.key.keysym.scancode == SDL_SCANCODE_KP_PLUS) ? 1 : -1;
                    if (game_speed == 0) game_speed = 1;
                    gamespeed_ms = (1000.0 / (float)game_speed);
                }
                else if ((event.key.keysym.scancode == SDL_SCANCODE_W))
                    temp_walls = !temp_walls;
                else if ((event.key.keysym.scancode == SDL_SCANCODE_F10))
                    valid = false;
                else{
                    return 1;
                }
                break;
            case SDL_QUIT:
                valid = false;
                break;
            }
        }


        render.RenderStart();

        render.RenderString("WELCOME TO", 0, 10);
        render.RenderString("\nS N E K  S I M U L A T O R  2 0 1 7");
        render.RenderString("\n\n\nSetup:");
        render.RenderString("\n\n +/-: In-game speed: ");
        render.RenderString(patch::to_string(game_speed));
        render.RenderString("\n   W: Solid walls: ");
        render.RenderString(patch::to_string(temp_walls));
        render.RenderString("\n\n\nIn-game:");
        render.RenderString("\n\n F10: Exit");
        render.RenderString("\n\nUse arrow keys to steer");
        render.RenderString("\n\n\n Press any other key to start");

        EndFrame();
    }
    return 0;
}

int GameWrapper::InGameLoop(){
    StartGame();
    // Fixed time step is used since the game should be so simple that anything but microwavers should be able to run it at 30/60FPS
    while (valid){
        StartFrame();
        PollEvents();       // Get events and send them to event handlers
        render.RenderStart();

        if (gamefield->GetGameState() == GAMESTATE_ACTIVE){
            gamefield->TimerUpdate(); // Update in-game timer
            if (frame_start - gamespeed_last > gamespeed_ms){
                gamefield->Update(); // Update field state
                gamespeed_last = frame_start;
            }
        }
        render.RenderField(field_obj, playa_obj);    // Render field and HUD
        render.RenderHUD(gamefield);
        if (gamefield->GetGameState() == GAMESTATE_PAUSED)
            render.RenderString("Paused!", 128, 128);
        EndFrame();
    }
    return 0;
}

int GameWrapper::MainLoop(){
    if (!valid)
        return -1;
    if (MainMenuLoop() == 1)
        InGameLoop();
    return 0;
}

int main(int argc, char **args){
    GameWrapper game;
    return game.MainLoop();
}
