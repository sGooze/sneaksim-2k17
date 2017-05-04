// sneke.hpp
// Purpose:

#pragma once

#include "common.hpp"
#include "render_element.hpp"
#include <list>
#include <forward_list>

// TODO: REMOVE RenderElement && RenderQueue
//       Rendering is handled directly by Renderer, which has two more functions: render basic object, and render sneke

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

    typedef wall sneke_body;

    class sneke{
    private:
        uint16_t x, y;
        uint16_t length = 5;
        DIRECTION movement_dir = DIR_LEFT;
        std::forward_list<sneke_body*> body;
    public:
        sneke(int cx, int cy);
        ~sneke();
        void move(DIRECTION dir_new);
        void move (const int& cx, const int& cy);
        void kill();
        void Collide(COLLISION& col);           // Updates state of the snek based on the recent collision result
        uint16_t GetX(){return x;}
        uint16_t GetY(){return y;}
        std::forward_list<sneke_body*>& GetBody(){return body;}
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

    class field{
    private:
        uint16_t x, y;
        uint32_t score = 0;
        object_list objects;
        sneke player;
    public:
        field(int size_x, int size_y) : x(size_x), y(size_y), player(size_x / 2, size_y / 2) {};
        ~field(){};

        void Update();
        void ParseEvent(SDL_Event& event);
        void AddObjectList();
        uint16_t GetX(){return x;}
        uint16_t GetY(){return y;}
        object_list* GetObjectListPtr(){return &objects;}
        object_list& GetObjectList(){return objects;}
        sneke* GetPlayerObjectPtr(){return &player;}
        sneke& GetPlayerObject(){return player;}
    };
}
