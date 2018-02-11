#ifndef _BOUKEN_POINT_
#define _BOUKEN_POINT_

class Point {
public:
	int x, y ;
	Point(int v) : x(v), y(v) {}
	Point(int x, int y) : x(x), y(y) {}
	int getLength() const {
		return x * y ;
	}
};

inline Point operator* (const Point& lhs, const Point& rhs){
	return Point(lhs.x * rhs.x, lhs.y * rhs.y);
}

inline Point operator/ (const Point& lhs, const Point& rhs){
	return Point(lhs.x / rhs.x, lhs.y / rhs.y);
}

inline Point operator+ (const Point& lhs, const Point& rhs){
	return Point(lhs.x + rhs.x, lhs.y + rhs.y);
}

inline Point operator- (const Point& lhs, const Point& rhs){
	return Point(lhs.x - rhs.x, lhs.y - rhs.y);
}

inline int getDistance(const Point& lhs, const Point& rhs){
	Point difference = lhs - rhs ;
	return difference.getLength();
}

#endif // _BOUKEN_POINT_
