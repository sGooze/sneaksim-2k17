//render2d.hpp

#pragma once

#include "common.hpp"
#include "sneke.hpp"
#include "pawn.hpp"

#define APPEND_STRING 65535

// TODO: Base Render class defined in the engine; custom renderers inside dlls are derived from it

class Render2D{
private:
    // engine
    SDL_Renderer *renderer = NULL;
    SDL_Window *window = NULL;
    uint16_t w, h;
    bool borderless = false;
    bool fullscreen = false;
    std::string win_title = "Sneak 2: Sneak Harder";
    // dll
    uint8_t scale = 7;
    SDL_Rect viewport_field;
    SDL_Rect viewport_hud;
    SDL_Texture *texField = NULL, *texFont = NULL;
public:
    // engine
    Render2D();
    ~Render2D();
    Render2D(uint16_t win_width, uint16_t win_height, bool accelerated = true); // Uses reset(), throws fatal exception on fail
    // dll
    void Reset(bool recreate = false);       // Recreate window and renderer with new parameters
    void InitFieldTexture(uint16_t fx, uint16_t fy);
    void InitFontTexture();

    // Main rendering functions // dll
    void RenderStart();
    void RenderField(std::list<Sneke_SM::object*>* objects = NULL, Sneke_SM::sneke* player = NULL);
    void RenderHUD(Sneke_SM::field* gamefield);
    void RenderEnd();
    void HandleEvent(SDL_Event& event);

    // Setup window parameters // engine
    void WindowSetBorderless(bool new_val){borderless = new_val;}
    void WindowSetFullscreen(bool new_val){fullscreen = new_val;}
    void WindowSetWidth(uint16_t new_val){w = new_val;}
    void WindowSetHeight(uint16_t new_val){h = new_val;}
    void WindowResize(){SDL_SetWindowSize(window, w, h);}
    void WindowSetTitle(std::string& title){win_title = title; SDL_SetWindowTitle(window, win_title.c_str());}

    // Get window parameters // engine
    std::string WindowGetTitle(){return win_title;}

    // Setup render parameters // dll
    void RenderSetScale(uint8_t scale_){scale = scale_; SDL_RenderSetScale(renderer, scale, scale);}

    // Drawing functions // engine
    void RenderDrawPoint(uint16_t x, uint16_t y);
    void RenderDrawLine(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2);
    void RenderDrawRect(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2);
    void RenderFillRect(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2);
    void RenderString(const std::string& str, uint16_t start_x = APPEND_STRING, uint16_t start_y = APPEND_STRING);

    //void RenderDrawPawn()

    // Drawing functions for game objects
    void RenderBasicPawn(BasicPawn* pawn);
};
