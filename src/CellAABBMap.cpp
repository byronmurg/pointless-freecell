#include <CellAABBMap.hpp>

void
CellAABBMap::forEachAtPoint
(const Point &point, CallbackType callback)
const
{
	forEach(
		[point, callback]
		(const Cell& cell, const AABB& aabb)
		{
			if ( aabb.containsPoint(point) )
				callback(cell, aabb);
		}
	);
}
