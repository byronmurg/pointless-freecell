#include <AABB.hpp>

AABB::AABB
()
	: _aa(0)
	, _bb(0)
{}

AABB::AABB
(const Point &aa, const Point &bb)
	: _aa(aa)
	, _bb(bb)
{}

bool
AABB::containsPoint
(const Point &toTest)
const noexcept
{
	return ( toTest.x > _aa.x )
		&& ( toTest.y > _aa.y )
		&& ( toTest.x < _bb.x )
		&& ( toTest.y < _bb.y )
	;
}

SDL::Rect
AABB::toSDLRect
()
const
{
	return SDL::Rect(_aa, _bb - _aa);
}

Point
AABB::getOffset
(const Point& point)
const
{
	return point - _aa ;
}
