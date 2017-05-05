// sneke.cpp
#include "inc/sneke.hpp"

//using namespace Sneke_SM;

////******* O B J E C T _ L I S T *******////

bool Sneke_SM::object_list::Add(object* obj){
    if (getObject(obj->GetX(), obj->GetY()) != NULL){
        return false;
    }
    objlist.push_back(obj);
    return true;
}

Sneke_SM::object* Sneke_SM::object_list::getObject(const int& cx, const int& cy){
    for (auto it = objlist.begin(); it != objlist.end(); ++it){
        if (((*it)->GetX() == cx)&&((*it)->GetY() == cy))
            return *it;
    }
    return NULL;
}

////******* S N E K E _ B O D Y *******////

// TODO: Replace with something. Do something.
// Akin to double-ended queue

#define DEFAULT_BODY_RESERVES 32

Sneke_SM::sneke_body::sneke_body(uint16_t length_, int& head_x, int& head_y) : length(length_){
    body.reserve(DEFAULT_BODY_RESERVES);
    wall body_temp(head_x, head_y);
    for(int i = 0; i < length; i++){
        body.push_back(body_temp);
    }
}

void Sneke_SM::sneke_body::Grow(){
    // TODO:
    body.push_back(body[last_piece]);
    last_piece = length++;
}

void Sneke_SM::sneke_body::Move(uint16_t& head_x, uint16_t& head_y){
    body[last_piece].SetXY(head_x, head_y);
    if (last_piece-- == 0)
        //last_piece = body.size() - 1;
        last_piece = length-1;
}

////******* S N E K E *******////

Sneke_SM::sneke::sneke(int cx, int cy) : x(cx), y(cy), body(5, cx, cy){
    // Construct snake body
}

Sneke_SM::sneke::~sneke(){
}


// // // // // // //
// F  I  E  L  D  //
// // // // // // //

void Sneke_SM::field::ParseEvent(SDL_Event& event){
    // Called once per rendering cycle
    if (event.type == SDL_KEYDOWN){
        switch(event.key.keysym.scancode){
            case SDL_SCANCODE_DOWN:  if (player.movement_dir != DIR_UP) player.movement_dir = DIR_DOWN; break;
            case SDL_SCANCODE_LEFT:  if (player.movement_dir != DIR_RIGHT) player.movement_dir = DIR_LEFT; break;
            case SDL_SCANCODE_RIGHT: if (player.movement_dir != DIR_LEFT) player.movement_dir = DIR_RIGHT; break;
            case SDL_SCANCODE_UP:    if (player.movement_dir != DIR_DOWN) player.movement_dir = DIR_UP; break;
            case SDL_SCANCODE_G: player.body.Grow(); break;
        }
    }
}

void Sneke_SM::field::Update(){
    // Called once per game update cycle
    // Move snek, then check for collisions and update states of snek and objects
    player.body.Move(player.x, player.y);
    switch (player.movement_dir){
        case DIR_DOWN:  (player.y < y) ? player.y++ : (player.y = 0); break;
        case DIR_LEFT:  (player.x > 0) ? player.x-- : (player.x = x); break;
        case DIR_RIGHT: (player.x < x) ? player.x++ : (player.x = 0); break;
        case DIR_UP:    (player.y > 0) ? player.y-- : (player.y = y); break;
    }
}
