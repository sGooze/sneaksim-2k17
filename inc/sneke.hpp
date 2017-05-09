// sneke.hpp
// Purpose:

#pragma once

#include "common.hpp"
#include "render_element.hpp"
#include <list>
#include <forward_list>
#include <vector>


namespace Sneke_SM{

    enum COLLISION {COLL_NONE, COLL_KILL, COLL_EAT};
    enum DIRECTION {DIR_UP = 0, DIR_RIGHT = 1, DIR_DOWN = 2, DIR_LEFT = 3};

    class object{
    protected:
        //uint16_t x, y;
        SDL_Rect bbox;
        SDL_Color color = {0xFF, 0xFF, 0xFF, 0xFF};
    public:
        virtual COLLISION onCollide() = 0;
        object(int cx, int cy) : bbox{cx, cy, 1, 1}{};
        virtual ~object(){};
        uint16_t GetX(){return bbox.x;}
        uint16_t GetY(){return bbox.y;}
        void SetXY(int& cx, int& cy){bbox.x = cx; bbox.y = cy;}
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
    };

    class field;

    class sneke{
    private:
        friend class field;
        uint16_t x, y;
        DIRECTION movement_dir = DIR_LEFT;

        class sneke_body{
        public:
            std::list<wall> body;
            uint16_t last_piece, length;

            sneke_body(uint16_t length_, int& head_x, int& head_y);
            void Grow();
            void Move(uint16_t& head_x, uint16_t& head_y);
            ~sneke_body(){};

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
        std::list<wall>& GetBody(){return body.body;};
    };

    class object_list{
    private:
        std::list<object*> objlist;
    public:
        object_list(){};
        COLLISION getCollision(const int& cx, const int& cy); // Gets object collision result; may remove it
        object* getObject(const int& cx, const int& cy);
        //std::list<object&> getObjectsByType()
        bool Add(object* obj);
        bool Remove(object* obj);                       // Finds object in the list, removes it and returns TRUE
    };

    enum field_gamestate {GAMESTATE_INACTIVE, GAMESTATE_ACTIVE, GAMESTATE_PAUSED, GAMESTATE_FINISHED};

    class field{
    private:
        uint16_t x, y;
        uint32_t score = 0;
        object_list objects;
        sneke player;
        field_gamestate game_state = GAMESTATE_INACTIVE;
    public:
        field() : x(0), y(0), player(0, 0){};
        field(int size_x, int size_y) : x(size_x), y(size_y), player(size_x / 2, size_y / 2) {game_state = GAMESTATE_ACTIVE;};
        ~field(){};

        void Init(int size_x, int size_y);
        void Update();                      // Moves snek, then checks for collision
        void ParseEvent(SDL_Event& event);
        void AddObjectList();
        uint16_t GetX(){return x;}
        uint16_t GetY(){return y;}
        uint32_t GetScore(){return score;}

        field_gamestate GetGameState(){return game_state;}
        void SetGameState(field_gamestate new_state){game_state = new_state;}

        object_list* GetObjectListPtr(){return &objects;}
        object_list& GetObjectList(){return objects;}
        sneke* GetPlayerObjectPtr(){return &player;}
        sneke& GetPlayerObject(){return player;}
    };
}
