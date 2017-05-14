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
    for (auto it = objlist.begin(); it != objlist.end(); it++){
        if (((*it)->GetX() == cx)&&((*it)->GetY() == cy))
            return *it;
    }
    return NULL;
}

Sneke_SM::object_list::~object_list(){
    for (auto it = objlist.begin(); it != objlist.end(); it++){
        delete *it;
    }
}

////******* S N E K E _ B O D Y *******////

Sneke_SM::sneke::sneke_body::sneke_body(uint16_t length_, int& head_x, int& head_y) : length(length_){
    wall body_temp(head_x, head_y);
    for(int i = 0; i < length; i++){
        body.push_back(body_temp);
    }
}

void Sneke_SM::sneke::sneke_body::Grow(){
    body.push_back(body.back());
    length++;
}

void Sneke_SM::sneke::sneke_body::Move(uint16_t& head_x, uint16_t& head_y){
    body.splice(body.begin(), body, --(body.end()));
    body.front().SetXY(head_x, head_y);
}

////******* S N E K E *******////

Sneke_SM::sneke::sneke(int cx, int cy) : x(cx), y(cy), body(5, cx, cy){
    // Construct snake body
}

Sneke_SM::sneke::~sneke(){}

bool Sneke_SM::sneke::IsCollidingWithBody(){
    for (Sneke_SM::wall& piece : body.body){
        if ((x == piece.GetX())&&(y == piece.GetY()))
            return true;
    }
    return false;
}

// // // // // // //
// F  I  E  L  D  //
// // // // // // //

Sneke_SM::field::field (int size_x, int size_y) : x(size_x), y(size_y), player(size_x / 2, size_y / 2),
                                            coords_x(0, size_x - 1), coords_y(0, size_y - 1){
    game_state = GAMESTATE_ACTIVE;
    randgen.seed(std::chrono::system_clock::now().time_since_epoch().count());
    SpawnTreat();
}

void Sneke_SM::field::ParseEvent(SDL_Event& event){
    // Called once per rendering cycle
    switch (game_state){
    case GAMESTATE_PAUSED:
        if ((event.type == SDL_KEYDOWN)&&(event.key.keysym.scancode == SDL_SCANCODE_ESCAPE))
            game_state = GAMESTATE_ACTIVE;
        return;
    case GAMESTATE_INACTIVE:
    case GAMESTATE_FINISHED:
        return;
    }
    if (event.type == SDL_KEYDOWN){
        switch(event.key.keysym.scancode){
            case SDL_SCANCODE_ESCAPE: game_state = GAMESTATE_PAUSED;
                std::cout << "POS " << player.x << " " << player.y << std::endl;
                break;
            // TODO: BUG: Pressing combinations such as LEFT->UP during one keyframe when snek is moving DOWN...
            // will lead to sneak's head moving into it's body
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
    static Sneke_SM::object *obj;
    // Move snek's body
    player.body.Move(player.x, player.y);
    // Move snek's head
    switch (player.movement_dir){
        // TODO: Optimize ( ) )
        case DIR_DOWN:  (player.y < y - 1) ? player.y++ : (player.y = 0); break;
        case DIR_LEFT:  (player.x > 0) ? player.x-- : (player.x = x - 1); break;
        case DIR_RIGHT: (player.x < x - 1) ? player.x++ : (player.x = 0); break;
        case DIR_UP:    (player.y > 0) ? player.y-- : (player.y = y - 1); break;
    }
    // First, check for collision with body
    if (player.IsCollidingWithBody()){
        game_state = GAMESTATE_FINISHED;
    }
    obj = objects.getObject(player.x, player.y);
    if (obj != NULL){
        switch (obj->onCollide()){
            case Sneke_SM::COLL_EAT:
                score += obj->GetValue();
                player.body.Grow();
                obj->SetXY( coords_x(randgen), coords_y(randgen) );
                break;
            case Sneke_SM::COLL_KILL:
                game_state = GAMESTATE_FINISHED;
                break;
        }
    }
}

void Sneke_SM::field::SpawnTreat(){
    Sneke_SM::fruit* candy = new Sneke_SM::fruit(0, 0);
        candy->SetXY( coords_x(randgen), coords_y(randgen) );
    objects.Add(candy);
    std::cout << "Spawned some candy at (" << candy->GetX() << ";" << candy->GetY() << ")\n";
}

