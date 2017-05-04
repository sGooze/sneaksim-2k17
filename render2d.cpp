#include "inc/render2d.hpp"

std::string title = "Sneak Simulator 2017";

Render2D::Render2D(){
    //Reset();
}

Render2D::~Render2D(){
    if (texField != NULL)
        SDL_DestroyTexture(texField);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
}

void Render2D::Reset(bool recreate){
    if (recreate){
        // Destroy window and renderer first
        if (renderer != NULL)
            SDL_DestroyRenderer(renderer);
        if (window != NULL)
            SDL_DestroyWindow(window);
        int win_param = SDL_WINDOW_INPUT_FOCUS;
        if (fullscreen)
            win_param |= SDL_WINDOW_FULLSCREEN;
        else if (borderless)
            win_param |= SDL_WINDOW_BORDERLESS;
        window = SDL_CreateWindow(win_title.c_str(), SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, w, h, win_param);
        if (window == NULL)
            throw std::runtime_error("Render2D::Reset() - unable to create a window\n");
        renderer = SDL_CreateRenderer(window, 0, SDL_RENDERER_ACCELERATED | SDL_RENDERER_TARGETTEXTURE);
        if (renderer == NULL)
            throw std::runtime_error("Render2D::Reset() - unable to create a renderer\n");
        InitFontTexture();
    } else {
        WindowResize();
        WindowSetBorderless(borderless);
        WindowSetFullscreen(fullscreen);
    }
    viewport_hud.w = w;
    viewport_hud.h = 32;
    viewport_hud.x = 0;
    viewport_hud.y = h - 32;
}

void Render2D::InitFieldTexture(uint16_t fx, uint16_t fy){
    if (texField != NULL)
        SDL_DestroyTexture(texField);
    //texField = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_RGB24, SDL_TEXTUREACCESS_TARGET, fx * scale, fy * scale);
    texField = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_RGB24, SDL_TEXTUREACCESS_TARGET, fx, fy );
    if (texField == NULL){
        std::cout << "Render2D::InitFieldTexture - an error as occurred: " << SDL_GetError() << std::endl;
        return;
    }

    viewport_field.w = fx * scale; viewport_field.h = fy * scale;
    // Viewport for the field should be located in the center of the screen (minding the HUD at the bottom)
    viewport_field.x = (w/2) - ((fx * scale)/2);
    viewport_field.y = (h/2) - ((fy * scale)/2);
    std::cout << "viewport_field - x: " <<  viewport_field.x << " y: " << viewport_field.y << " w: " << viewport_field.w << " h: " << viewport_field.h << std::endl;
}

void Render2D::InitFontTexture(){
    if (texFont != NULL)
        SDL_DestroyTexture(texFont);
    // TODO: Define font size based on texture proportions
    SDL_Surface *suf = SDL_LoadBMP("font.bmp");
    if (suf == NULL)
        throw std::runtime_error("Render2D::InitFontTexture - unable to find font file\n");
    texFont = SDL_CreateTextureFromSurface(renderer, suf);
    if (texFont == NULL)
        throw std::runtime_error("Render2D::InitFontTexture - unable to create font texture\n");
}

void Render2D::RenderDrawPoint(uint16_t x, uint16_t y){
    SDL_RenderDrawPoint(renderer, x, y);
}

void Render2D::RenderDrawLine(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2){
    SDL_RenderDrawLine(renderer, x1, y1, x2, y2);
}

void Render2D::RenderDrawRect(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2){
    static SDL_Rect rect;
    rect.x = x1; rect.y = y1;
    rect.w = x2 - x1; rect.h = y2 - y1;
    SDL_RenderDrawRect(renderer, &rect);
}

void Render2D::RenderFillRect(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2){
    static SDL_Rect rect;
    rect.x = x1; rect.y = y1;
    //rect.w = x2 - x1; rect.h = y2 - y1;
    rect.w = x2; rect.h = y2;
    SDL_RenderFillRect(renderer, &rect);
}

#define FONT_SIZEX 16
#define FONT_SIZEY 16

void Render2D::RenderString(const std::string& str, const uint16_t start_x, const uint16_t start_y){
    uint8_t ch;
    static SDL_Rect ch_bitmap = {0, 0, FONT_SIZEX, FONT_SIZEY}, ch_screen = ch_bitmap;
    ch_bitmap.x = ch_bitmap.y = 0;
    ch_screen.x = start_x; ch_screen.y = start_y;
    for (auto it = str.begin(); it < str.end(); ++it){
        ch = *it;
        if (ch == '\n'){
            ch_screen.x = start_x; ch_screen.y += FONT_SIZEY;
            continue;
        }
        if (ch < 32)
            ch = 32;
        if (ch > 128)
            ch = 128;
        // For fonts without first 32 chars
        ch -= 32;
        ch_bitmap.x = ch * FONT_SIZEX;
        SDL_RenderCopy(renderer, texFont, &ch_bitmap, &ch_screen);
        ch_screen.x += FONT_SIZEX;
    }
}

// // // // // // // // // // // // // // // //
// R E N D E R // G A M E  // O B J E C T S  //
// // // // // // // // // // // // // // // //

void Render2D::RenderObject(Sneke_SM::object* obj){
    SDL_Color& col = obj->GetColor();
    SDL_SetRenderDrawColor(renderer, col.r, col.g, col.b, col.a);
    //SDL_Rect& rect = obj.GetBBox();
    //SDL_RenderFillRect(renderer, &rect);
    SDL_RenderFillRect(renderer, &obj->GetBBox());
}

void Render2D::RenderSneak(Sneke_SM::sneke* sneak){
    SDL_SetRenderDrawColor(renderer, 0xFF, 0, 0, 0xFF);
    RenderDrawPoint(sneak->GetX(), sneak->GetY());
    Sneke_SM::wall* piece = sneak->GetBody();
    while (piece != NULL){
        RenderObject(piece);
        piece = sneak->GetBody();
    }
}

void Render2D::Render(Sneke_SM::object_list* objects, Sneke_SM::sneke* player){
    // Clear rendering space
    SDL_SetRenderDrawColor(renderer, 0x00, 0xFF, 0x00, 0xFF);
    SDL_RenderClear(renderer);
    // First, render field texture
    if (texField != NULL){
        if (!(SDL_SetRenderTarget(renderer, texField))){
            SDL_SetRenderDrawColor(renderer, 0x00, 0x00, 0x00, 0xFF);
            SDL_RenderClear(renderer);
            // Render all objects to the field texture
            if (objects != NULL){

            }
            if (player != NULL)
                RenderSneak(player);

            SDL_SetRenderTarget(renderer, NULL);
            //SDL_RenderSetViewport(renderer, &viewport_field);
            SDL_RenderSetScale(renderer, 1, 1);

            // Render HUD
            SDL_RenderSetViewport(renderer, &viewport_hud);
            SDL_SetRenderDrawColor(renderer, 0x00, 0xFF, 0xFF, 0xFF);
            RenderFillRect(0, 0, 640, 32);
            RenderString("HUD Test\nCurrent size: 32px (2 strings)\nShould be invisible", 0, 0);
            SDL_RenderSetViewport(renderer, NULL);

            SDL_RenderCopy(renderer, texField, NULL, &viewport_field);
            RenderString(title, 0, 0);
            //RenderString("i only plan to type\n whatever i want", 256, 256);

            SDL_RenderSetScale(renderer, scale, 1);
        }
    }
    // Then, render HUD
    // Bottom panel
    // On-screen HUD (menus)

    // Finish rendering
    SDL_RenderPresent(renderer);
}
