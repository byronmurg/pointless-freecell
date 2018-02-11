#ifndef _SDL_RECT_HPP_
#define _SDL_RECT_HPP_

#include <SDL.h>

#include <Point.hpp>

namespace SDL {

class Rect {
public:
	Rect(int, int, int, int);
	Rect(const Point&, const Point&);
	const SDL_Rect *getPointr() const { return &_data ; }
protected:
	SDL_Rect _data ;
};

}

#endif // _SDL_RECT_HPP_
