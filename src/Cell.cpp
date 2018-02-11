#include <Cell.hpp>

Cell::Cell()
	: _area(Area::null)
	, _column(-1)
	, _row(-1)
{}

Cell::Cell
(Area area, int column, int row)
	: _area(area)
	, _column(column)
	, _row(row)
{
	// @TODO: Make the exceptions different.
	if ( ( _area == Area::main ) && ( _column >= 8 ) )
		throw new InvalidPosition ;
	else if ( ( _column < 0 ) || ( _row < 0 ) )
		throw new InvalidPosition ;
	else if ( ( _area != Area::main ) && ( ( _column > 4 ) || ( _row ) ) )
		throw new InvalidPosition ;
	else if ( ( _area == Area::null ) && ( _column || _row ) )
		throw new InvalidPosition ;
}

std::string
Cell::getAsString
()
const
{
	if ( _area == Area::null )
		return getAreaString();
	else if ( _area == Area::main )
		return getAreaString() +" ("+ getColumnString() +":"+ getRowString() +")" ; 
	else 
		return getAreaString() +" ("+ getColumnString() +")" ; 
}

std::string
Cell::getAreaString
()
const
{
	if ( _area == Area::main )
		return "Main" ;
	else if ( _area == Area::sorted )
		return "Sorted" ; 
	else if ( _area == Area::freecell )
		return "FreeCell" ;
	else
		return "Null" ;
}

std::string
Cell::getColumnString
()
const
{
	return std::to_string(_column);
}

std::string
Cell::getRowString
()
const
{
	return std::to_string(_row);
}

int 
Cell::getAsInt
()
const
{
	return ( 1000 * static_cast<int>(_area) ) + ( _column * 100 ) + ( _row );
}

bool
Cell::isValid
()
const
{
	return _area != Area::null ;
}

bool
Cell::operator==
(const Cell& rhs)
const
{
	return ( _area == rhs._area )
		&& ( _column == rhs._column )
		&& ( _row == rhs._row )
	;
}

Cell
Cell::next
()
const
{
	if ( _area != Area::main )
		return Cell();
	else
		return Cell(getArea(), getColumn(), getRow() + 1);
}

Cell
Cell::prev
()
const
{
	if ( _area != Area::main )
		return Cell();
	else
		return Cell(getArea(), getColumn(), getRow() - 1);
}

bool
CellCompare::operator()
(const Cell& l, const Cell& r)
const
{
	return l.getAsInt() < r.getAsInt();
}

void
PossibleCells::forEachFreeCellAreaCell
(CallbackType callback)
{
	for ( int column = 0 ; column < 4 ; column++ ){
		Cell position(Cell::Area::freecell, column, 0);
		callback(position);
	}
}

void
PossibleCells::forEachMainAreaCell
(CallbackType callback)
{
	for ( int column = 0 ; column < 8 ; column++ ){
		for ( int row = 0 ; row < 40 ; row++ ){
			Cell position(Cell::Area::main, column, row);
			callback(position);
		}
	}
}

void
PossibleCells::forEachSortedAreaCell
(CallbackType callback)
{
	for ( int column = 0 ; column < 4 ; column++ ){
		Cell position(Cell::Area::sorted, column, 0);
		callback(position);
	}
}

void
PossibleCells::forEach
(CallbackType callback)
{
	forEachFreeCellAreaCell(callback);
	forEachMainAreaCell(callback);
	forEachSortedAreaCell(callback);
}

