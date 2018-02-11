#include <GameBoard.hpp>

void invalidMove(const std::string err){
	SDL::showMessage("Invalid Move", err);
}

GameBoard::GameBoard(RendererPtr renderer,
		int width,
		int height)
: _renderer(renderer), _cardImageLoader(renderer), _cardMap(),
	_aabbMap(), _cardSize(100, 140), _mouseDragStart(),
	_mousePosition(0), _cardGripOffset(0), _draggedCards(),
	_cardStubSize(30), _mainAreaColumnPadding(0), _topAreaPadding(0),
	_topAreaSeperatorWidth(20),_windowBorder(5),
	_gameCompleteHandler(nullptr), _mouseDown(false)
{
	handleWindowResize(width, height);
}

void GameBoard::startGame()
{
	checkGameCompleteHandlerSet();
	_cardMap.clear();
	populateCardMap();
	dealCards();
}

void GameBoard::checkGameCompleteHandlerSet() const
{
	if ( ! _gameCompleteHandler )
		throw GameCompleteHandlerNotSet(); 
}

void GameBoard::handleWindowResize(int width, int)
{
#ifndef NDEBUG
	std::cout << "Resizing to " << width << std::endl ;
#endif // NDEBUG

	int usableWidth = width - ( _windowBorder * 2 );
	_mainAreaColumnPadding = ( usableWidth - ( _cardSize.x * 8 ) ) / 8 ;

	if ( _mainAreaColumnPadding < 0 )
		_mainAreaColumnPadding = 0 ; 

	_topAreaPadding = 5 ;

	_topAreaSeperatorWidth = width - ( ( _cardSize.x * 8 ) + ( 10 * _topAreaPadding ));

	if ( _topAreaSeperatorWidth < 0 )
		_topAreaSeperatorWidth = 0 ;

	populateBoardAABBs();
}

void GameBoard::handleMouseDown(const Point&)
{
	_mouseDown = true ;
}

void GameBoard::handleMouseUp(const Point &point)
{
	_mouseDown = false ;

	if ( currentlyDragging() )
		handleMouseDrag(point);
}

void GameBoard::handleMouseMove(const Point& point)
{
	if ( _mouseDown && ! currentlyDragging() ){
		Cell cell = getTopCardCellAtPoint(point);
		if ( ! cell ) return ;

		AABB aabb = _aabbMap[cell];
		_cardGripOffset = aabb.getOffset(point);

		if ( _cardMap.cellIsDraggable(cell) )
			beginDrag(cell);
	}
	_mousePosition = point ;
}

void GameBoard::handleDoubleClick(const Point& point)
{
#ifndef NDEBUG
	std::cout << "In handleDoubleClick" << std::endl ; 
#endif // NDEBUG

	if ( currentlyDragging() )
		return ;

	_mouseDown = false ;

	Cell cell = getTopCardCellAtPoint(point);

	if ( ! cell )
		return ;
	else if ( cell.isInSortedArea() )
		return ;
	else if ( cell.isInFreeCellArea() )
		handleDoubleClickInFreeCellArea(cell);
	else if ( cell.isInMainArea() )
		handleDoubleClickInMainArea(cell);

	if ( canAutoSort() )
		autoSortAll();
}

void GameBoard::draw()
{
	drawBoard();
	drawDraggedCards();
}

void GameBoard::setGameCompleteHandler(std::function<void(void)> callback)
{
	_gameCompleteHandler = callback ;
}

void GameBoard::testGameComplete()
{
	if ( nKingsInSortedArea() == 4 )
		_gameCompleteHandler();
}

int GameBoard::nKingsInSortedArea() const
{
	int nSuitsSorted(0);
	_cardMap.forEachInSortedArea(
		[&nSuitsSorted]
		(const Cell &, const Card::Ptr &card)
		{
			if ( card && card->isKing() )
				nSuitsSorted += 1 ;
		}
	);
	return nSuitsSorted ;
}

void GameBoard::handleDoubleClickInMainArea(const Cell& cell)
{
	if ( isThisTopCellInStack(cell) ){
		if ( cardCanBeSorted(cell) )
			autoSortCard(cell);
		else
			moveCardToFirstFreeCell(cell);
	}
}

void GameBoard::handleDoubleClickInFreeCellArea(const Cell& cell)
{
	if ( cardCanBeSorted(cell) )
		autoSortCard(cell);
}

bool GameBoard::currentlyDragging() const
{
	return bool( _mouseDragStart );
}

void GameBoard::beginDrag(const Cell &startCell)
{
#ifndef NDEBUG
	std::cout << "In beginDrag" << std::endl ; 
#endif // NDEBUG
	_mouseDragStart = startCell ;
	Cell cell = startCell ;
	while ( _cardMap.cardSetInCell(cell) ){
		auto &card = _cardMap[cell] ;
		_draggedCards.emplace_back(card.release());
		cell = cell.next();
	}
}

void GameBoard::endDrag()
{
#ifndef NDEBUG
	std::cout << "In endDrag, returning " << _draggedCards.size() << " cards" << std::endl ; 
#endif // NDEBUG
	Cell cell = _mouseDragStart ;
	for ( auto &card : _draggedCards ){
		if ( ! card )
			continue ;
		_cardMap[cell] = std::move(card);
		cell = cell.next();
	}
	_draggedCards.clear();
	_mouseDragStart = Cell() ; // @TODO: Not clear.
}

void GameBoard::handleValidMouseDrag(const Cell &end)
{
#ifndef NDEBUG
	std::cout << "In handleValidMouseDrag" << std::endl ; 
#endif // NDEBUG

	if ( end.isInFreeCellArea() )
		handleDragToFreeCell(end);
	else if ( end.isInSortedArea() )
		handleDragToSortedArea();
	else if ( end.isInMainArea() )
		handleDragToMainArea(end);
	
	if ( canAutoSort() )
		autoSortAll();
}

void GameBoard::handleMouseDrag(const Point &point)
{
#ifndef NDEBUG
	std::cout << "In handleMouseDrag" << std::endl ; 
#endif // NDEBUG

	Cell end = getCellAtPoint(point);

#ifndef NDEBUG
	std::cout << "  target = " << end.getAsString() << std::endl ; 
#endif // NDEBUG

	if ( end )
		handleValidMouseDrag(end);

	endDrag();
}

void GameBoard::handleDragToMainArea(const Cell &end)
{
#ifndef NDEBUG
	std::cout << "drag to main stack column = " << end.getAsString() << std::endl ;
#endif //NDEBUG

	Cell stackStart = _cardMap.getTopPopulatedCellInColumn(end.getColumn());

	if ( stackStart )
		handleDragToMainAreaStack(stackStart);
	else
		handleDragToMainAreaEmptyStack(end);
}

void GameBoard::handleDragToMainAreaStack(const Cell &cell)
{
	auto &card = _cardMap[cell];
	auto next = cell.next();

	if ( _draggedCards.front()->canGoOnTopOf(*card) )
		performMainAreaDragIfAble(next);
}

void GameBoard::handleDragToMainAreaEmptyStack(const Cell &cell)
{
	Cell columnStart = Cell(Cell::Area::main, cell.getColumn(), 0);
	//if ( _mouseDragStart != columnStart )
		performMainAreaDragIfAble(columnStart);
}

void GameBoard::handleDragToFreeCell(const Cell &end)
{
#ifndef NDEBUG
	std::cout << "In handleDragToFreeCell" << std::endl ; 
#endif // NDEBUG

	if ( _draggedCards.size() > 1 ){
		invalidMove("Can only move one card to free cell");
	} else {
		auto &card = _draggedCards.front();
		moveCardToFreeCell(end, card);
	}
}

void GameBoard::handleDragToSortedArea()
{
	auto &card = _draggedCards.front();
	Cell target = getCardSortedTarget(card);
	auto &last = _cardMap[target];

	if ( _draggedCards.size() > 1 ){
		invalidMove("Cannot sort more than one card");
	} else if ( card->isAce() ||
		( last && card->isNextInSuitTo(*last) )){
		_cardMap[target] = std::move(_draggedCards[0]);
		_draggedCards.pop_back();
		testGameComplete();
	}
}

void GameBoard::performMainAreaDragIfAble(const Cell &cell)
{
	int effectiveFreeCells = getEffectiveFreeCells(cell),
	   nCardsDragged = _draggedCards.size();

	if ( nCardsDragged <= effectiveFreeCells ){
		performMainAreaDrag(cell);
	} else {
		invalidMove("Not enough free cells "
				+ std::to_string(nCardsDragged)
				+" required but you can only move "
				+ std::to_string(effectiveFreeCells));
	}

}

void GameBoard::performMainAreaDrag(const Cell &cell)
{
	Cell stackStart = cell ;
	for ( auto &card : _draggedCards ){
		_cardMap[stackStart] = std::move(card);
		stackStart = stackStart.next();
	}
	_draggedCards.clear();
}

size_t GameBoard::getEffectiveFreeCells(const Cell &targetCell) const
{
	int usableEmptyColumnModifier = nEmptyMainAreaColumns() + 1 ;
	if ( targetCell.getRow() == 0 ) usableEmptyColumnModifier -= 1 ;
	if ( _mouseDragStart.isInMainArea()
		&& _mouseDragStart.getRow() == 0 ) usableEmptyColumnModifier -= 1 ;
	int freeCells = _cardMap.nFreeCells() + 1 ;
	return freeCells * usableEmptyColumnModifier ;
}

size_t GameBoard::nEmptyMainAreaColumns() const
{
	size_t n = 0 ;
	_cardMap.forEachInMainArea(
		[&n]
		(const Cell& cell, const Card::Ptr& card)
		{
			if ( ( cell.getRow() == 0 ) && ! card )
				n += 1 ;
		}
	);
	return n ;
}

void GameBoard::dealCards()
{
	std::vector<Card::Ptr> newCards ;

	PossibleCards::forEach(
		[&newCards]
		(const Card& card)
		{
#ifndef NDEBUG
			std::cout << "adding card " << card.getName() << std::endl ;
#endif // NDEBUG
			newCards.emplace_back(new Card(card));
		}
	);

	shuffleCards(newCards);

	distributeCardsToBoard(newCards);
}

void GameBoard::shuffleCards(std::vector<Card::Ptr>& newCards)
{
	unsigned seed = std::chrono::system_clock::now().time_since_epoch().count();

	std::shuffle(newCards.begin(), newCards.end(), std::default_random_engine(seed));
}

void GameBoard::distributeCardsToBoard(std::vector<Card::Ptr>& newCards)
{
	int i = 0 ;
	for ( auto &card : newCards ){
		Cell cell(Cell::Area::main, i % 8, i / 8);
		_cardMap[cell] = std::move(card);
		i++ ;
	}
}

void GameBoard::drawDraggedCards()
{
	for ( size_t i = 0 ; i < _draggedCards.size() ; i++ ){
		auto &card = _draggedCards[i];
		if ( ! card ) continue ;
		Point cardPosition = _mousePosition - _cardGripOffset ;
		cardPosition.y += _cardStubSize * i ;
		SDL::Rect drawRect(cardPosition, _cardSize);

		auto texture = _cardImageLoader.getImage(*card);
		_renderer->copy(texture, drawRect);
	}
}

void GameBoard::drawBoard()
{
	_cardMap.forEachPopulatedCell(
		[this]
		(const Cell& cell, const Card::Ptr& card)
		{
			Rect rect = _aabbMap[cell].toSDLRect();
			TexturePtr texture = _cardImageLoader.getImage(*card);
			_renderer->copy(texture, rect);
			
		}
	);
}

bool GameBoard::cardCanBeSorted(const Cell &cell)
{
	auto &card = _cardMap[cell];
	Cell target = getCardSortedTarget(card);
	auto &targetCard = _cardMap[target];
	if ( ! isThisTopCellInStack(cell) )
		return false ;
	else if ( targetCard )
		return card->isNextInSuitTo(*targetCard);
	else
		return card->isAce();
}

bool GameBoard::canAutoSort()
{
	int nCompleteColumns(0), nIncompleteColumns(0);
	_cardMap.forEachInMainAreaFirstRowWithCard(
		[this, &nCompleteColumns, &nIncompleteColumns]
		(const Cell &cell, const Card::Ptr &card)
		{
			if ( card->isKing() && _cardMap.cellIsInMainAreaSequence(cell) )
				nCompleteColumns += 1 ;
			else
				nIncompleteColumns += 1 ;
		}
	);

	return nCompleteColumns == 4 && nIncompleteColumns == 0 ;
}

void GameBoard::autoSortAll()
{
	_cardMap.forEachInMainAreaHorizontallyWithCard(
		[this]
		(const Cell &cell, const Card::Ptr&)
		{
			if ( cardCanBeSorted(cell) )
				autoSortCard(cell);
		}
	);
}

void GameBoard::autoSortCard(const Cell& cell)
{
	auto &card = _cardMap[cell];
	Cell target = getCardSortedTarget(card);
	_cardMap[target] = std::move(card);
	testGameComplete();
}

void GameBoard::moveCardToFirstFreeCell(const Cell& cell)
{
#ifndef NDEBUG
	std::cout << "In moveCardToFirstFreeCell" << std::endl ;
#endif // NDEBUG

	Cell firstFreeCell = _cardMap.getFirstEmptyFreeCell();

	if ( ! firstFreeCell )
		return invalidMove("No free cells");

	auto &card = _cardMap[cell];
	_cardMap[firstFreeCell] = std::move(card);
}

void GameBoard::moveCardToFreeCell(const Cell &freeCell, Card::Ptr& card)
{
#ifndef NDEBUG
	std::cout << "In moveCardToFreeCell" << std::endl ;
	std::cout << "Moving " << card->getName() << " to " << freeCell.getAsString() << std::endl ;
#endif // NDEBUG

	if ( _cardMap.cardSetInCell(freeCell) )
		invalidMove("cell not free");
	else
		_cardMap[freeCell] = std::move(card);
}

void GameBoard::forEachCellAtPointWithCard(const Point& point,
		AABBCallbackType callback) const
{
	_aabbMap.forEachAtPoint(
		point,
		[this, callback]
		(const Cell& cell, const AABB& aabb)
		{
			if ( _cardMap.cardSetInCell(cell) )
				callback(cell, aabb);
		}
	);
}

bool GameBoard::isThisTopCellInStack(const Cell& cell) const
{
	Cell oneAbove = cell.next();
	return ! _cardMap.cardSetInCell(oneAbove);
}

Cell GameBoard::getCellAtPoint(const Point& point) const
{
	Cell maxCell ;
	_aabbMap.forEachAtPoint(
		point,
		[this, &maxCell]
		(const Cell& cell, const AABB&)
		{
			if ( ! cell.isInMainArea() )
					maxCell = cell ;
			else if ( cell.getRow() > maxCell.getRow() )
					maxCell = cell ;
		}
	);
	return maxCell ;
}

Cell GameBoard::getTopCardCellAtPoint(const Point& point) const
{
	Cell maxCell ;

	forEachCellAtPointWithCard(
		point,
		[this, &maxCell]
		(const Cell& cell, const AABB&)
		{
			if ( ! cell.isInMainArea() )
					maxCell = cell ;
			else if ( cell.getRow() > maxCell.getRow() )
					maxCell = cell ;
		}
	);
	return maxCell ;
}

Cell GameBoard::getCardSortedTarget(const Card::Ptr& card) const
{
	auto suit = card->getSuit(); 
	int columnN = static_cast<int>(suit);
	return Cell(Cell::Area::sorted, columnN, 0);
}


void GameBoard::populateCardMap()
{
	PossibleCells::forEach(
		[this]
		(const Cell& cell)
		{ _cardMap[cell]; }
	);
}

void GameBoard::populateBoardAABBs()
{
	populateBoardFreeCellAreaAABBs();
	populateBoardSortedAreaAABBs();
	populateBoardMainAreaAABBs();
}

void GameBoard::populateBoardFreeCellAreaAABBs()
{
	Point offset(_windowBorder, _windowBorder);
	int offsetPerCard = _cardSize.x + _topAreaPadding ;

	addAABBsForSelection(
			PossibleCells::forEachFreeCellAreaCell,
			offset,
			Point(offsetPerCard, 0)
			);
}

void GameBoard::populateBoardSortedAreaAABBs()
{
	Point offset = getSortedAreaOffset();
	int offsetPerCard = _cardSize.x + _topAreaPadding ;

	addAABBsForSelection(
			PossibleCells::forEachSortedAreaCell,
			offset,
			Point(offsetPerCard, 0)
			);
}

void GameBoard::populateBoardMainAreaAABBs()
{
	Point offset = getMainAreaOffset();
	offset.x += _mainAreaColumnPadding / 2 ; 
	int xOffsetPerCard = _cardSize.x + _mainAreaColumnPadding ;

	addAABBsForSelection(
			PossibleCells::forEachMainAreaCell,
			offset,
			Point(xOffsetPerCard, _cardStubSize)
			);
}

void GameBoard::addAABBsForSelection(
		std::function<void(AABBSelector)> selector,
		const Point& startOffset,
		const Point& increment)
{
	selector(
		[this, startOffset, increment]
		(const Cell& cell)
		{
			Point aa = startOffset ;
		   	aa.x += cell.getColumn() * increment.x ;
		   	aa.y += cell.getRow() * increment.y ;
			_aabbMap[cell] = AABB(aa, aa + _cardSize);
		}
	);
}

Point GameBoard::getSortedAreaOffset() const
{
	Point offset(_windowBorder, _windowBorder);
	int offsetPerCard = _cardSize.x + _topAreaPadding ;
	offset.x += offsetPerCard * 4 ; 
	offset.x += _topAreaPadding ; 
	offset.x += _topAreaSeperatorWidth ;
	return offset ;
}

Point GameBoard::getMainAreaOffset() const
{
	Point offset(_windowBorder, _windowBorder);
	offset.y += _cardSize.y ;
	return offset ;
}
