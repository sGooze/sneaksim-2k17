// sdl_app.hpp
// PURPOSE: Main app class. Handles SDL init/shutdown and main game loop control

#pragma once

#include <SDL2/SDL.h>
#include "common.hpp"
#include "sneke.hpp"
#include "render2d.hpp"

using namespace Sneke_SM;

#define FPS_MAX 30.0

class GameWrapper{
private:
    bool valid = false;
    bool InitSDL();
    bool ShutdownSDL();

    Render2D render;
    field *gamefield = NULL;
    std::list<object*> *field_obj = NULL;
    sneke *playa_obj = NULL;

    uint16_t game_speed = 5; // in frames per second
    bool temp_walls = false;

    uint32_t frame_start, gamespeed_last = 0;
     int16_t sleep;
    uint16_t frame_ms, gamespeed_ms;
public:
    GameWrapper();
    ~GameWrapper();
    void PollEvents();      // Gets all events from SDL event pipeline and sends them to handlers
    void StartGame();       // Init gamefield
    void StopGame();        // Set gamefield state as inactive

    int MainLoop(); // Read events, process events, tick the field, calculate framerate and wait
    int InGameLoop();
    int MainMenuLoop();
};

extern std::string title;
