// velocity.cpp
// All code related to the velocity-based movement test

#include "inc/pawn.hpp"
#include "inc/render2d.hpp"


bool BasicPawn::Move(double gamespeed){
    if (vel.isNull())
        return false;
    //pos = pos + (vel * gamespeed);
    pos = pos + (vel);
    vel *= 0.9;
    //vel *= (0.01 * gamespeed);
    return true;
}

void PlayerPawn::ShootBullet(){
    static int cursor_x, cursor_y;
    //SDL_GetRelativeMouseState(&cursor_x, &cursor_y);
    SDL_GetMouseState(&cursor_x, &cursor_y);

    crosshair.x = cursor_x;
    crosshair.y = cursor_y;
    crosshair.z = 15;
}

bool PlayerController::ParseEvent(SDL_Event& event){
    /*vec3& velocity = player_pawn->GetVelocity();
    switch(event.key.keysym.scancode){
        case SDL_SCANCODE_S:    velocity.y =  1; break;
        case SDL_SCANCODE_A:    velocity.x = -1; break;
        case SDL_SCANCODE_D:    velocity.x =  1; break;
        case SDL_SCANCODE_W:    velocity.y = -1; break;
    }*/
    return true;
}

#define BASE_SPEED 4.0

bool PlayerController::SetPlayerInputToPawn(){
    static uint32_t last_shoot = 0, current_shoot;

    vec3& velocity = player_pawn->GetVelocity();
    if (kbState.GetButtonState(SDL_SCANCODE_S))  velocity.y =   BASE_SPEED;
    if (kbState.GetButtonState(SDL_SCANCODE_A))  velocity.x = - BASE_SPEED;
    if (kbState.GetButtonState(SDL_SCANCODE_D))  velocity.x =   BASE_SPEED;
    if (kbState.GetButtonState(SDL_SCANCODE_W))  velocity.y = - BASE_SPEED;
    if (kbState.GetButtonState(SDL_SCANCODE_SPACE)){
        current_shoot = SDL_GetTicks();
        if (current_shoot - last_shoot > 20){
            last_shoot = current_shoot;
            player_pawn->ShootBullet();
        }
    }
    player_pawn->Move(1000.0 / 30.0);
    return true;
}
