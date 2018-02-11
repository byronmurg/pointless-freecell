#ifndef _BOARD_POSITION_
#define _BOARD_POSITION_


#include <exception>
#include <functional>
#include <string>

struct InvalidPosition : public std::exception {
	const char * what() noexcept {
		return "Invalid position" ;
	}
};

class Cell {
public:

	enum class Area {
		null = 0,
		freecell = 1,
		sorted = 2,
		main = 3
	};

	std::string getAsString() const ;
	std::string getAreaString() const ;
	std::string getColumnString() const ;
	std::string getRowString() const ;

	int getAsInt() const ;
	bool isValid() const ;
	operator bool() const { return isValid(); }
	bool operator !() const { return ! isValid(); }
	bool operator ==(const Cell&) const ;

	Area getArea() const { return _area ; }
	int getColumn() const { return _column ; }
	int getRow() const { return _row ; }

	Cell next() const ;
	Cell prev() const ;

	bool isInMainArea() const noexcept { return _area == Area::main ; }
	bool isInFreeCellArea() const noexcept { return _area == Area::freecell ; }
	bool isInSortedArea() const noexcept { return _area == Area::sorted ; }

	Cell();
	Cell(const Cell&) = default ;
	Cell(Area, int, int);
private:
	Area _area ;
	int _column, _row ;
};

struct CellCompare {
	bool operator()(const Cell&, const Cell&) const ;
};

class PossibleCells {
public:
	typedef std::function<void(const Cell&)> CallbackType ;

	static void forEachFreeCellAreaCell(CallbackType);
	static void forEachMainAreaCell(CallbackType);
	static void forEachSortedAreaCell(CallbackType);

	static void forEach(CallbackType);
};

#endif // _BOARD_POSITION_
