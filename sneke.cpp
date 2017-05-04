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

Sneke_SM::sneke_body::sneke_body(int length, int& head_x, int& head_y){
    head = new sneke_body_element(head_x, head_y);
    head->next = new sneke_body_element(head_x, head_y);
    current = prtail = head;
    for (int i = 0; i < length-2; i++)
        Grow();
}

void Sneke_SM::sneke_body::Grow(){
    prtail->next->next = new Sneke_SM::sneke_body::sneke_body_element(prtail->next->piece.GetX(), prtail->next->piece.GetY());
    prtail = prtail->next;
}

void Sneke_SM::sneke_body::Move(uint16_t& head_x, uint16_t& head_y){
    prtail->next->next = head->next;
    head = prtail->next;
    prtail->next = NULL;
    static int x = head_x, y = head_y;
    head->piece.SetXY(x, y);
}

////******* S N E K E *******////

Sneke_SM::sneke::sneke(int cx, int cy) : x(cx), y(cy), body(5, cx, cy){
    // Construct snake body
}

Sneke_SM::sneke::~sneke(){
    // Since sneke's body was constructed from pointers, it needs to be manually deleted
}


// // // // // // //
// F  I  E  L  D  //
// // // // // // //

void Sneke_SM::field::ParseEvent(SDL_Event& event){
    // Called once per rendering cycle
}

void Sneke_SM::field::Update(){
    // Called once per game update cycle
    // Move snek, then check for collisions and update states of snek and objects
    // Snake's head gets new coordinates based on movement direction
    // Last piece of snake's tail is moved to the beginning and takes the former place of snake's head
    switch (player.movement_dir){
        case DIR_DOWN:  (player.y < y) ? player.y++ : (player.y = 0); break;
        case DIR_LEFT:  (player.x > 0) ? player.x-- : (player.x = x); break;
        case DIR_RIGHT: (player.x < x) ? player.x++ : (player.x = 0); break;
        case DIR_UP:    (player.y > 0) ? player.y-- : (player.y = y); break;
    }
    player.body.Move(player.x, player.y);
}
