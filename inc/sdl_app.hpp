// sdl_app.hpp
// PURPOSE: Main app class. Handles SDL init/shutdown and main game loop control

#pragma once

#include <SDL2/SDL.h>
#include "common.hpp"
#include "sneke.hpp"
#include "render2d.hpp"


class GameWrapper{
private:
    bool valid = false;
    bool InitSDL();
    bool ShutdownSDL();

    Render2D render;

    uint16_t game_speed = 2; // in frames per second
    uint16_t frametime_prev;
    uint16_t frametime_d;
    uint16_t frametime_now;
public:
    GameWrapper();
    ~GameWrapper();
    void PollEvents();      // Gets all events from SDL event pipeline and sends them to handlers

    int MainLoop(); // Read events, process events, tick the field, calculate framerate and wait
};

extern std::string title;
