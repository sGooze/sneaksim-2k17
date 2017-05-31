// velocity.cpp
// All code related to the velocity-based movement test

#include "inc/pawn.hpp"
#include "inc/render2d.hpp"


bool BasicPawn::Move(double gamespeed){
    if (vel.isNull())
        return false;
    //pos = pos + (vel * gamespeed);
    pos = pos + (vel);
    vel *= 0.01;
    //vel *= (0.01 * gamespeed);
    return true;
}

bool PlayerController::ParseEvent(SDL_Event& event){
        vec3& velocity = player_pawn->GetVelocity();
        switch(event.key.keysym.scancode){
            case SDL_SCANCODE_S:    velocity.y =  1; break;
            case SDL_SCANCODE_A:    velocity.x = -1; break;
            case SDL_SCANCODE_D:    velocity.x =  1; break;
            case SDL_SCANCODE_W:    velocity.y = -1; break;
        }
    return true;
}
