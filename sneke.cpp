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

////******* S N E K E *******////

Sneke_SM::sneke::sneke(int cx, int cy) : x(cx), y(cy){
    // Construct snake body
    // At the start of the game snake body extends to the right of snake's head in a straight line
    // OR
    // At the start of the game snake body is compressed into a single point around it's head
    // It slowly (and naturally) unwraps itself after that
    for (int i = 0; i < length; i++){
        sneke_body *sn = new sneke_body(cx + i + 1, cy);
        //sneke_body *sn = new sneke_body(cx, cy);
        body.push_front(sn);
    }
}

Sneke_SM::sneke::~sneke(){
    // Since sneke's body was constructed from pointers, it needs to be manually deleted
    for ( auto it = body.begin(); it != body.end(); it++ )
        delete *it;
}

void Sneke_SM::sneke::move(Sneke_SM::DIRECTION dir_new){
    // Snake's head gets new coordinates based on movement direction
    // Last piece of snake's tail is moved to the beginning and takes the former place of snake's head
    /*static auto it = body.end();
    (*it).bbox.x = x;
    (*it).bbox.y = y;
    movement_dir = dir_new;
    switch (movement_dir){
        case DIR_DOWN: y++; break;
        case DIR_LEFT: x--; break;
        case DIR_RIGHT: x++; break;
        case DIR_UP: y--; break;
    }*/
}

void Sneke_SM::sneke::kill(){

}

void Sneke_SM::sneke::Collide(COLLISION& col){

}
