// render_element.hpp
// PURPOSE: Used to safely communicate with rendering subsystem

#pragma once

#include "common.hpp"
#include <vector>

struct RenderElement{
    SDL_Rect rect;
    SDL_Color color;

    RenderElement(){};
    RenderElement(SDL_Rect& rect_, SDL_Color& col_) : rect(rect_), color(col_){};
    RenderElement(int x, int y, int w, int h, SDL_Color& col_) : color(col_){
        rect.x = x; rect.y = y; rect.w = w; rect.h = h;
    }
    RenderElement(int x, int y, int w, int h, uint8_t r, uint8_t g, uint8_t b, uint8_t a){
        rect.x = x; rect.y = y; rect.w = w; rect.h = h;
        color.r = r; color.g = g; color.b = b; color.a = a;
    }
};

/*
class RenderQueue{
// TODO: Alternate version: just a list of RenderElement objects that is passed directly to the renderer
// TODO: All event queues may be based on this????
    const uint16_t size = 256;
    RenderElement queue[size];
    uint16_t cur = 0;
public:
    RenderQueue(uint16_t max_size){};
    bool Push(RenderElement element){if (cur < size-1) queue[cur++] = element; else return false; return true;};
    RenderElement* Pop(){return (cur > 0) ? queue[cur--] : NULL;};
    void Clear(){cur = 0;}
};
*/

class RenderQueue{
    std::vector<RenderElement> queue;
    uint16_t cur;
public:
    RenderQueue(uint16_t start_size) : queue(start_size){};
    bool Push(RenderElement element){try{ queue.at(cur++) = element; } catch(std::out_of_range){queue.push_back(element);} return true;};
    RenderElement* Pop(){return (cur > 0) ? &queue[cur--] : NULL;};
    void Clear(){cur = 0;}
};
