#include "inc/sdl_app.hpp"

KeyboardState kbState;
MsgServer msgserver;

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
        /*case SDL_WINDOWEVENT:
            if ((event.window.event == SDL_WINDOWEVENT_FOCUS_LOST)&&(gamefield->GetGameState() == GAMESTATE_ACTIVE))
                gamefield->SetGameState(GAMESTATE_PAUSED);
            break;*/
        case SDL_KEYDOWN:
        case SDL_KEYUP:
            // TODO: Keydown/keyup events update keyboard object state; pressed keys are then passed to necessary objects
            if ((event.key.keysym.scancode == SDL_SCANCODE_F10))
                valid = false;
            kbState.SetButtonState( event.key.keysym.scancode, (event.type == SDL_KEYDOWN) );
            break;
        case SDL_QUIT:
            valid = false;
            break;
        }
    }
}

void GameWrapper::StartGame(){
    pcon.SetPawnPtr(new PlayerPawn( vec3(), vec3() ));
}

void GameWrapper::StopGame(){
    pcon.SetPawnPtr(NULL);
}

int GameWrapper::MainMenuLoop(){
    // not so loopy anymore
    StartFrame();
    render.RenderStart();

    render.RenderString("WELCOME TO", 0, 10);
    render.RenderString("\nS N E K  S I M U L A T O R  2 0 1 7 :  2");
    render.RenderString("\n        V E L O C I T Y  S N E K        ");
    render.RenderString("\n          a new way of sneaking         ");
    render.RenderString("\n\n\nSetup:");
    render.RenderString("\n\n +/-: In-game speed: ");
    render.RenderString("\n   W: Solid walls: ");
    render.RenderString("\n\n\nIn-game:");
    render.RenderString("\n\n F10: Exit");
    render.RenderString("\n\nUse arrow keys to steer");
    render.RenderString("\n\n\n Press any other key to start");

    EndFrame();
    SDL_Delay(500);
    return 1;
}

int GameWrapper::InGameLoop(){
    StartGame();
    PlayerPawn *player_pawn = pcon.GetPawnPtr();
    if (player_pawn == NULL)
        valid = false;
    // TODO: timing = frametime_desired / frametime_current
    while (valid){
        StartFrame();
        PollEvents();       // Get events and send them to event handlers
        render.RenderStart();

        pcon.SetPlayerInputToPawn();
        //player_pawn->Move(frame_ms); // Update field state

        render.RenderPlayerPawn(player_pawn);    // Render field and HUD
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
