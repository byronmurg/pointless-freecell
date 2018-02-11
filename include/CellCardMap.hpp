#ifndef _CELL_CARD_MAP_HPP_
#define _CELL_CARD_MAP_HPP_

#include <memory>
#include <Card.hpp>
#include <CellMap.hpp>

class CellCardMap : public _base_CellMap<Card::Ptr> {
public:
	//CellCardMap() : _base_CellMap(){}
	bool cardSetInCell(const Cell&) const ;
	size_t nFreeCells() const ;

	class PerformIfPopulated {
	public:
		PerformIfPopulated(CallbackType callback)
			: _callback(callback)
		{}

		void operator()(const Cell &cell, const Card::Ptr &card){
			if ( card )
				_callback(cell, card);
		}
	private:
		CallbackType _callback ;
	};

	void forEachEmptyFreeCell(std::function<void(const Cell&)>) const ;
	void forEachInMainAreaHorizontallyWithCard(CallbackType);
	void forEachInMainAreaFirstRowWithCard(CallbackType) const ;
	//void forEachInMainAreaColumnWithCard(CallbackType) const ;
	void forEachPopulatedCell(CallbackType) const ;
	Cell getFirstEmptyFreeCell() const ;
	Cell getTopPopulatedCellInColumn(int) const ;
	bool cellIsDraggable(const Cell&);
	bool cellIsInMainAreaSequence(const Cell& cell);
};

#endif // _CELL_CARD_MAP_HPP_
