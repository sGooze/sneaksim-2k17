#include "inc/render2d.hpp"
#include <cstdlib>

std::string title = "Sneak Simulator 2017";

// // // // // // // // // // // // // // // // //
//    F     //    O     //    N     //    T     //
// // // // // // // // // // // // // // // // //

BitmapFont::BitmapFont(std::string& filename){
    // Font size is assumed to be specified in the file name [filename format MUST be "font-WxH.bmp"]
    // Font size is extracted from file name
    size_t delim_pos = filename.find('x');
    if ((filename.substr(0, 5) != "font-")||(delim_pos == std::string::npos))
        throw std::runtime_error("BitmapFont::BitmapFont - improper filename format: must be \"font-WxH.bmp\"\n");
    sizeX = std::stoi(filename.substr( 5, delim_pos - 5 ));
    sizeY = std::stoi(filename.substr(delim_pos + 1, filename.find('.') - delim_pos));
    // Basic check is done with comparison of the specified height and the actual texture's height
    SDL_Surface *suf = SDL_LoadBMP(filename.c_str());
    if (suf == NULL)
        throw std::runtime_error("BitmapFont::BitmapFont - unable to find font file (font.bmp)\n");
    texFont = SDL_CreateTextureFromSurface(renderer, suf);
    if (texFont == NULL)
        throw std::runtime_error("BitmapFont::BitmapFont - unable to concert font surface to font texture\n");
    int sizeY_queried; SDL_QueryTexture(texFont, NULL, NULL, NULL, &sizeY_queried);
    if (sizeY != sizeY_queried)
        std::cout << "BitmapFont::BitmapFont - warning: height of the texture " << filename << "(" << sizeY_queried << ") does not match the height specified in the filename (" << sizeY << ")"
            << ". This may cause rendering artifacts to appear.\n";
}


// // // // // // // // // // // // // //
// R E N D E R //    2     //    D     //
// // // // // // // // // // // // // //

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
        // TODO: Adjust field texture & HUD positions on window resize
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
        std::cout << "Render2D::InitFieldTexture - an error has occurred: " << SDL_GetError() << std::endl;
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
        throw std::runtime_error("Render2D::InitFontTexture - unable to find font file (font.bmp)\n");
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

void Render2D::RenderString(const std::string& str, uint16_t start_x, uint16_t start_y){
    uint8_t ch;
    static SDL_Rect ch_bitmap = {0, 0, FONT_SIZEX, FONT_SIZEY}, ch_screen = ch_bitmap;
    static uint16_t string_startx = 0, string_starty = 0;
    ch_bitmap.x = ch_bitmap.y = 0;
    if (start_x != APPEND_STRING)
        ch_screen.x = string_startx = start_x;
    else
        start_x = ch_screen.x;
    if (start_y != APPEND_STRING)
        ch_screen.y = string_starty = start_y;
    else
        start_y = ch_screen.y;
    for (auto it = str.begin(); it < str.end(); ++it){
        ch = *it;
        if (ch == '\n'){
            ch_screen.x = string_startx; ch_screen.y += FONT_SIZEY;
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

void Render2D::RenderBasicPawn(BasicPawn* pawn){
    static SDL_Rect temp_rect = {0, 0, 5, 5};
    SDL_SetRenderDrawColor(renderer, 0xFF, 0xFF, 0xFF, 0xFF);
    vec3& pos = pawn->GetPosition();
    temp_rect.x = pos.x - 2;
    temp_rect.y = pos.y - 2;
    SDL_RenderDrawRect(renderer, &temp_rect);
    SDL_SetRenderDrawColor(renderer, 0xFF, 0xBB, 0x00, 0xFF);
    SDL_RenderDrawPoint(renderer, pos.x, pos.y);
}

void Render2D::RenderPlayerPawn(PlayerPawn* pawn){
    RenderBasicPawn(pawn);
    vec3& pos = pawn->GetPosition();
    vec3& cross = pawn->GetCrosshair();
    if (cross.z > 0){
        SDL_RenderDrawLine(renderer, (int)pos.x, (int)pos.y, (int)cross.x, (int)cross.y);
        cross.z--;
    }
}

// // // // // // // // // // // // // // //
// R E N D E R I N G // F U N C T I O N S //
// // // // // // // // // // // // // // //

void Render2D::RenderStart(){
    // Clear rendering space
    //SDL_SetRenderDrawColor(renderer, 0x00, 0xFF, 0x00, 0xFF);
    SDL_SetRenderDrawColor(renderer, 0x00, 0x00, 0x00, 0xFF);
    SDL_RenderClear(renderer);
}

void Render2D::RenderHUD(Sneke_SM::field* gamefield){
    //static Sneke_SM::sneke *sneak = gamefield->GetPlayerObjectPtr();
    // Render HUD
    /*SDL_RenderSetViewport(renderer, &viewport_hud);
    SDL_SetRenderDrawColor(renderer, 0x00, 0xFF, 0xFF, 0xFF);
    RenderFillRect(0, 0, 640, 32);
    RenderString("Time: " + patch::to_string(gamefield->GetGameTime() / 1000), 0, 0);
    RenderString("Score: " + patch::to_string(gamefield->GetScore()), 0, FONT_SIZEY);
    SDL_RenderSetViewport(renderer, NULL);
    RenderString(title, 0, 0);
    if (gamefield->GetGameState() == Sneke_SM::GAMESTATE_FINISHED){
        RenderString("GAME     OVER!\n Your score:" + patch::to_string(gamefield->GetScore()), 128, 128);
        RenderString("Press R to restart the game", 128, 200);
    }*/
}

void Render2D::RenderEnd(){
    SDL_RenderPresent(renderer);
}
