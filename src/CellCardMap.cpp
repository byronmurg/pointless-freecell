#include <CellCardMap.hpp>
#include <iostream>

bool
CellCardMap::cardSetInCell
(const Cell& cell)
const 
{
	if ( ! cell.isValid() ) return false ;
	const auto it = _map.find(cell);
	return ( it != _map.end() ) && bool(it->second) ;
}

void
CellCardMap::forEachEmptyFreeCell
(std::function<void(const Cell&)> callback)
const
{
	forEachFreeCell(
		[callback]
		(const Cell& cell, const Card::Ptr& card)
		{
			if ( ! card )
				callback(cell);
		}
	);
}

void
CellCardMap::forEachInMainAreaHorizontallyWithCard
(CallbackType callback)
{
	PerformIfPopulated check(callback);
	forEachInMainAreaHorizontally(check);
}

void
CellCardMap::forEachInMainAreaFirstRowWithCard
(CallbackType callback)
const
{
	PerformIfPopulated check(callback);
	forEachInMainAreaFirstRow(check);
}

void
CellCardMap::forEachPopulatedCell
(CallbackType callback)
const
{
	PerformIfPopulated check(callback);
	forEach(check);
}

size_t
CellCardMap::nFreeCells
()
const
{
	int n = 0 ;
	forEachEmptyFreeCell(
		[&n]
		(const Cell&)
		{  n++ ; }
	);
	return n ;
}

Cell
CellCardMap::getFirstEmptyFreeCell
()
const
{
	Cell ret ;
	forEachEmptyFreeCell(
		[&ret]
		(const Cell& cell)
		{
			if ( ret ) return ;
			ret = cell ;
		}
	);
	return ret ;
}   

Cell 
CellCardMap::getTopPopulatedCellInColumn
(int column)
const
{
	int row(-1);

	forEachInMainAreaColumn(
		column,
		[&row]
		(const Cell &cell, const Card::Ptr& card)
		{
			if ( ! card )
				return ;
			else if ( cell.getRow() > row )
				row = cell.getRow();
		}
	);

	if ( row < 0 )
		return Cell() ;

	return Cell(Cell::Area::main, column, row);
}

bool
CellCardMap::cellIsDraggable
(const Cell& cell)
{
	if ( cell.isInFreeCellArea() )
		return true ;
	else if ( cell.isInSortedArea() )
		return false ;
	else if ( cellIsInMainAreaSequence(cell) )
		return true ;
	else
		return false ;
}

bool
CellCardMap::cellIsInMainAreaSequence
(const Cell& checkCell)
{
	if ( ! checkCell.isInMainArea() ) return false ;

	Card *lastCard = _map[checkCell].get();
	
	if ( ! lastCard ) return false ;

	bool ret = true ;

	forEachInMainAreaColumn(
		checkCell.getColumn(),
		[&lastCard, checkCell, &ret]
		(const Cell& cell, const Card::Ptr& card)
		{
			if ( cell.getRow() <= checkCell.getRow() )
				return ;
			else if ( ! card )
				return ;
			else if ( card->canGoOnTopOf(*lastCard) )
				lastCard = card.get() ;
			else
				ret = false ;
		}
	);

	return ret ;
}

