// common.hpp
// PURPOSE: common include file, visible to everyone

#pragma once

#include <iostream>
#include <SDL2/SDL.h>

#include <exception>
#include <stdexcept>

#include <sstream>
#include <vector>


namespace patch
{
    // http://stackoverflow.com/questions/12975341/to-string-is-not-a-member-of-std-says-g-mingw
    // thank you, mingw
    template < typename T > std::string to_string( const T& n )
    {
        std::ostringstream stm ;
        stm << n ;
        return stm.str() ;
    }
}

class vec3{
    //float _x, _y, _z;
public:
    float x, y, z;
    vec3(float x_val = 0, float y_val = 0, float z_val = 0) : x(x_val), y(y_val), z(z_val){};
    vec3& operator+(vec3 add){x += add.x; y += add.y; z += add.z; return *this;}
    vec3& operator-(vec3 add){x -= add.x; y -= add.y; z -= add.z; return *this;}
    vec3& operator*(double mul){x *= mul; y *= mul; z *= mul; return *this;}
    vec3& operator*=(float mul){x *= mul; y *= mul; z *= mul; return *this;}
    vec3& operator*=(double mul){x *= mul; y *= mul; z *= mul; return *this;}
    bool isNull(float z_base = 0){return ( (x == 0)&&(y == 0)&&(z == z_base) );}

    void print(std::string prefix = ""){std::cout << prefix << x << ", " << y << ", " << z << std::endl;}

    //vec3(float x_val = 0, float y_val = 0, float z_val = 0) : _x(x_val), _y(y_val), _z(z_val){};
    //float& x() {return _x;}
    //float& y() {return _y;}
    //float& z() {return _z;}
};

