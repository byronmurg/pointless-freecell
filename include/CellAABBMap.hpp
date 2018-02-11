#ifndef _BOARD_AABB_Map_
#define _BOARD_AABB_Map_

#include <CellMap.hpp>
#include <Point.hpp>
#include <AABB.hpp>

class CellAABBMap : public _base_CellMap<AABB> {
public:
	void forEachAtPoint(const Point&, CallbackType) const ;
};

#endif // _BOARD_AABB_Map_
