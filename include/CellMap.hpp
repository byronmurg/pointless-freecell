#ifndef _BOARD_POSITION_MAP_
#define _BOARD_POSITION_MAP_

#include <map>
#include <functional>
#include <Point.hpp>
#include <Cell.hpp>

template <class T>
class _base_CellMap {
public:

	_base_CellMap() : _map() {}

	typedef std::function<void(const Cell&,const T&)> CallbackType ;
	typedef Cell::Area BoardArea ;
	typedef std::map<Cell, T, CellCompare> MapType ;

	void forEach(CallbackType callback) const {
		for ( const auto& pair : _map )
			callback(pair.first, pair.second);
	}

	void forEachInArea(BoardArea area, CallbackType callback) const {
		forEach([area, &callback](const Cell& position, const T& t){
			if ( position.getArea() == area )
				callback(position, t);
		});
	}

	void forEachFreeCell(CallbackType callback) const {
		return forEachInArea(BoardArea::freecell, callback);
	}

	void forEachInMainArea(CallbackType callback) const {
		return forEachInArea(BoardArea::main, callback);
	}

	void forEachInSortedArea(CallbackType callback) const {
		return forEachInArea(BoardArea::sorted, callback);
	}

	void forEachInMainAreaRow(int rowNumber, CallbackType callback) const {
		forEachInMainArea(
			[rowNumber, &callback]
			(const Cell& position, const T& t)
			{
				if ( position.getRow() == rowNumber )
					callback(position, t);
			}
		);
	}

	void forEachInMainAreaFirstRow(CallbackType callback) const {
		forEachInMainAreaRow(0, callback);
	}

	void forEachInMainAreaColumn(int columnNumber, CallbackType callback) const {
		forEachInMainArea(
			[columnNumber, &callback]
			(const Cell& position, const T& t)
			{
				if ( position.getColumn() == columnNumber )
					callback(position, t);
			}
		);
	}

	void forEachInMainAreaHorizontally(CallbackType callback){
		for ( int row = 25 ; row > -1 ; row-- )
			for ( int col = 0 ; col < 8 ; col++ ){
				Cell cell(BoardArea::main, col, row);
				callback(cell, _map[cell]);
			}
	}

	struct NullPositionPassedToCellMap : public std::exception {
		const char * what() const noexcept
		{ return "Null Position Passed To Board Position Map" ; }
	}; 

	T& operator[](const Cell& position){
		if ( ! position )
			throw new NullPositionPassedToCellMap ;
		return _map[position];
	}

	size_t size() const {
		return _map.size();
	}

	template <class ...Args>
	void emplace(Args ...args){
		_map.emplace(args...);
	}

	typename MapType::const_iterator find(const Cell &position) const {
		return _map.find(position);
	}

	typename MapType::const_iterator end() const {
		return _map.end();
	}

	void clear(){
		_map.clear();
	}

protected:
	MapType _map ;
};


#endif // _BOARD_POSITION_MAP_
