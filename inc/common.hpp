// common.hpp
// PURPOSE: common include file, visible to everyone

#pragma once

#include <iostream>
#include <SDL2/SDL.h>

#include <exception>
#include <stdexcept>

#include <sstream>


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
