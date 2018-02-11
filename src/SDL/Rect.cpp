#include <SDL/Rect.hpp>

namespace SDL {

Rect::Rect(int x, int y, int w, int h)
	: _data()
{
	_data.x = x ;
	_data.y = y ;
	_data.w = w ;
	_data.h = h ;
}

Rect::Rect(const Point& position, const Point& dimensions)
	: _data()
{
	_data.x = position.x ;
	_data.y = position.y ;
	_data.w = dimensions.x ;
	_data.h = dimensions.y ;
}

}
