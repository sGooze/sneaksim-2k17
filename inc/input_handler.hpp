// input_handler.hpp
// PURPOSE: Keyboard buttons state tracker

#pragma once

#include "common.hpp"

// TODO: Keyboard input handler (and other input handlers) are hidden inside one object, which queries inputs every frame and sends appropriate generic messages (like BUTTON_FIRE_PRESSED)

class KeyboardState{
    bool keys[SDL_NUM_SCANCODES] = {};
public:
    KeyboardState(){};
    //bool GetButtonState(SDL_Scancode scancode){if (scancode < SDL_NUM_SCANCODES) return keys[scancode]; else return false;}
    bool GetButtonState(SDL_Scancode scancode){return keys[scancode];}
    void SetButtonState(SDL_Scancode scancode, bool state){keys[scancode] = state;}
};

extern KeyboardState kbState;
