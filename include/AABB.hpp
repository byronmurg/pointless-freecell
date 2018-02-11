#ifndef _AABB_HPP_
#define _AABB_HPP_

#include <Point.hpp>
#include <SDL/Rect.hpp>

class AABB {
public:
	AABB();
	AABB(const Point &, const Point &);

	bool containsPoint(const Point&) const noexcept ;
	SDL::Rect toSDLRect() const ;
	Point getOffset(const Point&) const ;

	Point _aa, _bb ;
private:
};

#endif // _AABB_HPP_
