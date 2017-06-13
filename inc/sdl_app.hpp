// sdl_app.hpp
// PURPOSE: Main app class. Handles SDL init/shutdown and main game loop control

#pragma once

#include <SDL2/SDL.h>
#include "common.hpp"
#include "pawn.hpp"
#include "render2d.hpp"
#include "input_handler.hpp"

#define FPS_MAX 30.0
#define TIMING_DESIRED (double)(1000.0 / (double) FPS_MAX )

class GameWrapper{
private:
    bool valid = false;
    bool InitSDL();
    bool ShutdownSDL();

    Render2D render;

    PlayerController pcon;

    uint32_t frame_start, gamespeed_last = 0;
     int16_t sleep;
    uint16_t frame_ms, gamespeed_ms;
public:
    GameWrapper();
    ~GameWrapper();
    void PollEvents();      // Gets all events from SDL event pipeline and sends them to handlers
    void StartGame();       // Init gamefield
    void StopGame();        // Set gamefield state as inactive

    void StartFrame(){frame_start = SDL_GetTicks();};
    void EndFrame(){render.RenderEnd(); sleep = frame_start + frame_ms - SDL_GetTicks(); if (sleep < 0) sleep = 0; SDL_Delay(sleep);};

    int MainLoop(); // Read events, process events, tick the field, calculate framerate and wait
    int MainMenuLoop();
    int InGameLoop();
};

extern std::string title;
