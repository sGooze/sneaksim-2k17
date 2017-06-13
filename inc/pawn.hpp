// pawn.hpp - basic pawn class

#pragma once

#include "common.hpp"
#include "input_handler.hpp"

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
// Class: PlayerPawn ********************
//***************************************
// Description:
//  Pawn representing player. Can shoot
//***************************************

class PlayerPawn : public BasicPawn{
    vec3 crosshair;
public:
    PlayerPawn(vec3 position, vec3 velocity) : BasicPawn(position, velocity){};
    void ShootBullet();
    vec3& GetCrosshair(){return crosshair;}
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
    PlayerPawn *player_pawn;
public:
    PlayerController(PlayerPawn *player = NULL) : player_pawn(player) {};
    bool ParseEvent(SDL_Event& event);
    bool SetPlayerInputToPawn();            // Queries input devices's state and
    bool SetPawnPtr(PlayerPawn *new_player){if (player_pawn != NULL) delete player_pawn; player_pawn = new_player; return true;}
    PlayerPawn *GetPawnPtr(){return player_pawn;};
};
