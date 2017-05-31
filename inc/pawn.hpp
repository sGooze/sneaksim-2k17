// pawn.hpp - basic pawn class

#pragma once

#include "common.hpp"

//***************************************
// Class: BasicPawn *********************
//***************************************
// Description:
//  Basic moving pawn class. Pawn is an object
//  which can be controlled by the player or AI controller.
//***************************************

class BasicPawn{
    vec3 pos, vel;
    float face_angle;

    //Sector *in_sector;        // Sector in which the pawn is located
public:
    BasicPawn(vec3 position, vec3 velocity) : pos(position), vel(velocity){};

    vec3& GetPosition(){return pos;};
    vec3& GetVelocity(){return vel;};
    float GetAngle(){return face_angle;};

    bool Move(double gamespeed);                // Calculates new position based on current position, velocity and time multiplier; then checks validity of this position in current sector
    void SetVelocity(vec3 velocity){vel = velocity;};
    void AddVelocity(vec3 velocity){vel = vel + velocity;};
};

//***************************************
// Class: PlayerController **************
//***************************************
// Description:
//  Player controller gets inputs from player
//  and translates them to commands that are given
//  to the player pawn
//***************************************

class PlayerController{
    BasicPawn *player_pawn;
public:
    PlayerController(BasicPawn *player = NULL) : player_pawn(player) {};
    bool ParseEvent(SDL_Event& event);
    bool SetPawnPtr(BasicPawn *new_player){if (player_pawn != NULL) delete player_pawn; player_pawn = new_player; return true;}
    BasicPawn *GetPawnPtr(){return player_pawn;};
};
