// sneke.hpp
// Purpose:

#pragma once

#include "common.hpp"
#include "render_element.hpp"
#include <list>
#include <forward_list>
#include <vector>
#include <random>
#include <chrono>


namespace Sneke_SM{

    enum COLLISION {COLL_NONE, COLL_KILL, COLL_EAT};
    enum DIRECTION {DIR_UP = 0, DIR_RIGHT = 1, DIR_DOWN = 2, DIR_LEFT = 3, DIR_NONE = -1};

    enum ANIMATION_STYLE    {ANIM_NONE,     // No animation
                            ANIM_SWITCH};   // Switch between colors without transition

    /*/
    *** CLASS:
    ***     OBJECT - In-game object
    /*/

    class object{
    protected:
        //uint16_t x, y;
        SDL_Rect bbox;
        SDL_Color color = {0xFF, 0xFF, 0xFF, 0xFF};
        SDL_Color color2 = {0xFF, 0xFF, 0xFF, 0xFF};
        ANIMATION_STYLE anim_style = ANIM_NONE;
        uint8_t anim_time = 1;
    public:
        virtual COLLISION onCollide() = 0;
        virtual uint16_t GetValue(){return 0;}
        object(int cx, int cy) : bbox{cx, cy, 1, 1}{};
        virtual ~object(){};
        uint16_t GetX(){return bbox.x;}
        uint16_t GetY(){return bbox.y;}
        void SetXY(int cx, int cy){bbox.x = cx; bbox.y = cy;}
        void SetXY(uint16_t& cx, uint16_t& cy){bbox.x = cx; bbox.y = cy;}
        SDL_Rect& GetBBox(){return bbox;}
        SDL_Color& GetColor(){return color;}
    };

    class wall : public object{
    public:
        wall(int cx, int cy, int cwidth = 1, int cheight = 1) : object(cx, cy){bbox.w = cwidth; bbox.h = cheight;};
        virtual COLLISION onCollide() {return COLL_KILL;}
    };

    class fruit : public object{
        uint16_t value;
    public:
        fruit(int cx, int cy, uint16_t val_points = 100) : object(cx, cy), value(val_points){};
        COLLISION onCollide(){return COLL_EAT;}
        uint16_t GetValue(){return value;}
    };

    /*/
    *** CLASS:
    ***     OBJECT_LIST - stores and provides interface for adding, retrieving and deleting objects
    /*/

    class object_list{
    private:
        std::list<object*> objlist;
    public:
        object_list(){};
        ~object_list();
        COLLISION getCollision(const int& cx, const int& cy); // Gets object collision result; may remove it
        object* getObject(const int& cx, const int& cy);
        //std::list<object&> getObjectsByType()
        std::list<object*>* getAllObjects(){return &objlist;}
        bool Add(object* obj);
        bool Remove(object* obj);                       // Finds object in the list, removes it and returns TRUE
    };

    /*/
    *** CLASS:
    ***     SNEKE - object, representing player
    /*/

    class field;

    class sneke{
    private:
        friend class field;
        float x, y;
        float velocity_x = 1, velocity_y = 1;
        DIRECTION movement_dir = DIR_LEFT;
        //std::list<DIRECTION> movement_dirs;

        class sneke_body{
            // TODO: rewrite as a child of object_list??
        public:
            std::list<wall> body;
            uint16_t last_piece, length;

            sneke_body(uint16_t length_, int& head_x, int& head_y);
            void Grow();
            void Move(uint16_t& head_x, uint16_t& head_y);
            //void PushMoveDir(DIRECTION new_direction);          // Adds new movement direction to movement_dirs list
            //DIRECTION PopMoveDir();                             // Returns first movement direction from the queue. Returns DIR_NONE if queue is empty

            uint16_t GetLength(){return length;}
        };

        sneke_body body;
    public:
        sneke(int cx, int cy);
        ~sneke();
        //void Collide(COLLISION& col);           // Updates state of the snek based on the recent collision result
        bool IsCollidingWithBody();
        uint16_t GetX(){return x;}
        uint16_t GetY(){return y;}
        uint16_t GetLength(){return body.length;}
        void SetLength(uint16_t len);
        void Reset(uint16_t cx, uint16_t cy);    // Places snek to the specified position with its body coiled up in one spot
        std::list<wall>& GetBody(){return body.body;};
    };

    /*/
    *** CLASS:
    ***     FIELD - handles gameplay logic and stores all related objects
    /*/

    enum field_gamestate {GAMESTATE_INACTIVE, GAMESTATE_ACTIVE, GAMESTATE_PAUSED, GAMESTATE_FINISHED};

    class field{
    private:
        uint16_t x, y;
        uint32_t score = 0, game_pause = 0, timestamp_activated;
        object_list objects;
        sneke player;
        field_gamestate game_state = GAMESTATE_INACTIVE;
        DIRECTION dir_new = DIR_NONE;

        // Random number generator (for object coordinates)
        std::default_random_engine randgen;
        std::uniform_int_distribution<int> coords_x;
        std::uniform_int_distribution<int> coords_y;

    public:
        //field() : x(0), y(0), player(0, 0){};
        field(int size_x, int size_y, bool solid_edges = false);
        ~field(){};

        void Update();                      // Moves snek, then checks for collision
        void TimerUpdate(){/*game_time = SDL_GetTicks() - timestamp_activated;*/}
        void ParseEvent(SDL_Event& event);
        void AddObjectList();
        void SpawnTreat();                  // Temp function for basic candy spawning
        uint16_t GetX(){return x;}
        uint16_t GetY(){return y;}
        uint32_t GetScore(){return score;}
        uint32_t GetGameTime(){return /*SDL_GetTicks() - timestamp_activated + game_pause*/0;}

        field_gamestate GetGameState(){return game_state;}
        void SetGameState(field_gamestate new_state){game_state = new_state; if (game_state == GAMESTATE_ACTIVE) timestamp_activated = SDL_GetTicks();}

        object_list* GetObjectListPtr(){return &objects;}
        object_list& GetObjectList(){return objects;}
        sneke* GetPlayerObjectPtr(){return &player;}
        sneke& GetPlayerObject(){return player;}
    };
}
