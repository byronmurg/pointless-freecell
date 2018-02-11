#ifndef _GAME_BOARD_
#define _GAME_BOARD_

#include <map>
#include <functional>
#include <memory>
#include <vector>
#include <algorithm>
#include <chrono>
#include <random>
#include <string>

#ifndef NDEBUG
#include <iostream>
#endif // NDEBUG

#include <SDL.hpp>
#include <CellCardMap.hpp>
#include <CellAABBMap.hpp>
#include <Card.hpp>
#include <CardImageLoader.hpp>
#include <AABB.hpp>

using namespace SDL ;

void invalidMove(const std::string);

class GameBoard {
public:

	class GameCompleteHandlerNotSet {
		const char *what() const noexcept {
			return "Game complete handler was never set." ;
		}
	};

	using AABBCallbackType = CellAABBMap::CallbackType ; 
	using CardCallbackType = CellCardMap::CallbackType ; 

	GameBoard(RendererPtr, int, int);
	void startGame();
	void checkGameCompleteHandlerSet() const ;
	void handleWindowResize(int, int);
	void handleMouseDown(const Point&);
	void handleMouseUp(const Point&);
	void handleMouseMove(const Point&);
	void handleDoubleClick(const Point&);
	void draw();
	void setGameCompleteHandler(std::function<void(void)>);

protected:

	void testGameComplete();
	int nKingsInSortedArea() const ;
	void handleDoubleClickInMainArea(const Cell&);
	void handleDoubleClickInFreeCellArea(const Cell&);

	bool currentlyDragging() const ;
	void beginDrag(const Cell&);
	void endDrag();
	void handleValidMouseDrag(const Cell&);
	void handleMouseDrag(const Point&);

	void handleDragToMainArea(const Cell&);
	void handleDragToMainAreaStack(const Cell&);
	void handleDragToMainAreaEmptyStack(const Cell&);
	void handleDragToFreeCell(const Cell&);
	void handleDragToSortedArea();

	void dealCards();
	void shuffleCards(std::vector<Card::Ptr>&);
	void distributeCardsToBoard(std::vector<Card::Ptr>&);

	bool isCardSetAtCell (const Cell&) const ;
	bool isThisTopCellInStack(const Cell&) const ;
	bool cardCanBeSorted(const Cell&);
	bool canAutoSort();
	void autoSortAll();
	void autoSortCard(const Cell&);

	void moveCardToFirstFreeCell(const Cell&);
	void moveCardToFreeCell(const Cell&, Card::Ptr &);

	void performMainAreaDragIfAble(const Cell&);
	void performMainAreaDrag(const Cell&);
	size_t getEffectiveFreeCells(const Cell&) const ;
	size_t nEmptyMainAreaColumns() const ;

	void forEachCellAtPointWithCard(const Point&, AABBCallbackType) const ;
	Cell getCellAtPoint(const Point&) const ;
	Cell getTopCardCellAtPoint(const Point&) const ;
	Cell getCardSortedTarget(const Card::Ptr&) const ;

	void populateCardMap();
	void populateBoardAABBs();
	void populateBoardFreeCellAreaAABBs();
	void populateBoardSortedAreaAABBs();
	void populateBoardMainAreaAABBs();
	using AABBSelector = std::function<void(const Cell&)> ;
	void addAABBsForSelection(std::function<void(AABBSelector)>, const Point&, const Point&);

	Point getSortedAreaOffset() const ;
	Point getMainAreaOffset() const ;

	void drawDraggedCards();
	void drawBoard();

private:

	RendererPtr _renderer ;
	CardImageLoader _cardImageLoader ;
	CellCardMap _cardMap ;
	CellAABBMap _aabbMap ;

	Point _cardSize ;

	Cell _mouseDragStart ;
	Point _mousePosition, _cardGripOffset ;
	std::vector<Card::Ptr> _draggedCards ;

	int _cardStubSize, _mainAreaColumnPadding, _topAreaPadding,
		_topAreaSeperatorWidth, _windowBorder ;

	std::function<void(void)> _gameCompleteHandler ;

	bool _mouseDown ;
};

#endif // _GAME_BOARD_
