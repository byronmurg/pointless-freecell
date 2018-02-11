#include <Card.hpp>

bool Card::canGoOnTopOf(const Card& other) const noexcept {
#ifndef NDEBUG
	std::cout << "checking " << getName()
		<< " can go on " << other.getName()
		<< std::endl ;
#endif // NDEBUG
	return isOneAbove(other) && isDifferentColour(other) ;
}

bool Card::isDifferentColour(const Card& other) const noexcept {
	return getColour() != other.getColour();
}

bool Card::isOneAbove(const Card& other) const noexcept {
	return static_cast<int>(other._number) == ( static_cast<int>(_number) + 1 );
}

bool Card::isOneBelow(const Card& other) const noexcept {
	return static_cast<int>(_number) == ( static_cast<int>(other._number) + 1 );
}


bool
Card::isNextInSuitTo
(const Card& other)
const noexcept
{
	return isSameSuit(other) && isOneBelow(other);
}

bool
Card::isSameSuit
(const Card& other)
const noexcept
{
	return _suit == other._suit ;
}

std::string Card::getName() const {
	return getNumberName() +" of "+ getSuitName();
}

std::string Card::getSuitName() const {
	if ( _suit == Suit::SPADES )
		return "spades" ;
	else if ( _suit == Suit::CLUBS )
		return "clubs" ;
	else if ( _suit == Suit::HEARTS )
		return "hearts" ;
	else
		return "diamonds" ;
}

std::string Card::getNumberName() const {
	std::string ret ;
	switch ( _number ){
		case Number::ACE 	: ret = "ace" ; 	break ;
		case Number::TWO 	: ret = "two" ; 	break ;
		case Number::THREE 	: ret = "three" ; 	break ;
		case Number::FOUR 	: ret = "four" ; 	break ;
		case Number::FIVE 	: ret = "five" ; 	break ;
		case Number::SIX 	: ret = "six" ; 	break ;
		case Number::SEVEN 	: ret = "seven" ; 	break ;
		case Number::EIGHT 	: ret = "eight" ; 	break ;
		case Number::NINE 	: ret = "nine" ; 	break ;
		case Number::TEN 	: ret = "ten" ; 	break ;
		case Number::JACK 	: ret = "jack" ; 	break ;
		case Number::QUEEN 	: ret = "queen" ; 	break ;
		case Number::KING 	: ret = "king" ; 	break ;
	}
	return ret ;
}

Card::Colour Card::getColour() const noexcept {
	Colour ret ;
	switch ( _suit ){
		case Suit::CLUBS :
		case Suit::SPADES :
			ret = Colour::BLACK ;
			break ;
		case Suit::HEARTS :
		case Suit::DIAMONDS :
			ret = Colour::RED ;
			break ;
	}
	return ret ;
}

void PossibleCards::forEach(CallbackType callback){
	for ( int suit = 0 ; suit < 4 ; suit += 1  )
		for ( int number = 1 ; number <= 13 ; number += 1 )
			callback(
				Card(static_cast<Card::Suit>(suit),
					static_cast<Card::Number>(number))
				);
}

