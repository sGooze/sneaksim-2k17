// sdl_app.hpp
// PURPOSE: Main app class. Handles SDL init/shutdown and main game loop control

#pragma once

#include <SDL2/SDL.h>
#include "common.hpp"
#include "sneke.hpp"

class Render2D{
private:
    SDL_Renderer *renderer = NULL;
    SDL_Window *window = NULL;
    uint16_t w, h;
    uint8_t scale = 1;
    bool borderless = false;
    bool fullscreen = false;
    std::string win_title = "Sneak";

    SDL_Rect viewport_field;
    SDL_Rect viewport_hud;
    SDL_Texture *texField = NULL, *texFont = NULL;
public:
    Render2D();
    ~Render2D();
    Render2D(uint16_t win_width, uint16_t win_height, bool accelerated = true); // Uses reset(), throws fatal exception on fail
    void Reset(bool recreate = false);       // Recreate window and renderer with new parameters
    void InitFieldTexture(uint16_t fx, uint16_t fy);
    void InitFontTexture();

    // Setup window parameters
    void WindowSetBorderless(bool new_val){borderless = new_val;}
    void WindowSetFullscreen(bool new_val){fullscreen = new_val;}
    void WindowSetWidth(uint16_t new_val){w = new_val;}
    void WindowSetHeight(uint16_t new_val){h = new_val;}
    void WindowResize(){SDL_SetWindowSize(window, w, h);}
    void WindowSetTitle(std::string& title){win_title = title; SDL_SetWindowTitle(window, win_title.c_str());}

    // Get window parameters
    std::string WindowGetTitle(){return win_title;}

    // Setup render parameters
    void RenderSetScale(uint8_t scale_){scale = scale_; SDL_RenderSetScale(renderer, scale, scale);}

    // Drawing functions
    void RenderDrawPoint(uint16_t x, uint16_t y);
    void RenderDrawLine(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2);
    void RenderDrawRect(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2);
    void RenderFillRect(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2);
    void RenderString(const std::string& str, const uint16_t start_x, const uint16_t start_y);

    void Render();
};

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
