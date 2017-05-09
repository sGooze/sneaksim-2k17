//render2d.hpp

#pragma once

#include "common.hpp"
#include "sneke.hpp"
/*
class Texture{
private:
    SDL_Texture *tex = NULL;
    string path;
    uint16_t x, y;
    // uint8_t alpha_mod;
    // SDL_BlendMode blend_mode;                            All accessed through SDL_Get.. & SDL_Set.. functions, not stored in this class
    // uint8_t color_mod_r, color_mod_g, color_mod_b;
public:
    Texture(const std::string& path);
    Texture(uint32_t format, int access, int w, int h);
    ~Texture(){SDL_DestroyTexture(tex);}
    bool Load();
    bool Unload();
    SDL_Texture* GetTexturePtr() {return tex;};
};

class TextureArray{

public:
    TextureArray(){};

    bool Add(const std::string& name);          // Loads new texture from the list. Throws a warning and loads default placeholder on failure.
    Texture& Get(const std::string& name);      // Returns texture from the list. If it's not loaded, loads it first.
};
*/

class Render2D{
private:
    SDL_Renderer *renderer = NULL;
    SDL_Window *window = NULL;
    uint16_t w, h;
    uint8_t scale = 5;
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

    // Main rendering functions
    void RenderStart();
    void RenderField(Sneke_SM::object_list* objects = NULL, Sneke_SM::sneke* player = NULL);
    void RenderHUD(Sneke_SM::field* gamefield);
    void RenderEnd();
    void HandleEvent(SDL_Event& event);

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

    // Drawing functions for game objects
    void RenderObject(Sneke_SM::object* obj);
    void RenderSneak(Sneke_SM::sneke* sneak);

};
