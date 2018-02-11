#ifndef _CARD_
#define _CARD_

#include <string>
#ifndef NDEBUG
#include <iostream>
#endif // NDEBUG
#include <functional>
#include <memory>

class Card {
public:

	using Ptr = std::unique_ptr<Card> ;

	enum class Suit {
		CLUBS = 0, HEARTS = 1, SPADES = 2, DIAMONDS = 3
	};

	enum class Number {
		ACE = 1,
		TWO = 2,
		THREE = 3,
		FOUR = 4,
		FIVE = 5,
		SIX = 6,
		SEVEN = 7,
		EIGHT = 8,
		NINE = 9,
		TEN = 10,
		JACK = 11,
		QUEEN = 12,
		KING = 13
	};

	enum class Colour {
		RED, BLACK
	};

	Card(Suit suit, Number number)
		: _suit(suit)
		, _number(number)
	{};

	bool canGoOnTopOf(const Card &) const noexcept ;

	bool isDifferentColour(const Card &) const noexcept ;
	bool isOneAbove(const Card &) const noexcept ;

	bool isNextInSuitTo(const Card&) const noexcept ;
	bool isSameSuit(const Card&) const noexcept ;
	bool isOneBelow(const Card&) const noexcept ;

	bool isKing() const { return _number == Number::KING ; }
	bool isAce() const { return _number == Number::ACE ; }

	Suit getSuit() const { return _suit ; }

	std::string getName() const ;
	std::string getSuitName() const ;
	std::string getNumberName() const ;

	Colour getColour() const noexcept ;

private:
	Suit _suit ;
	Number _number ;
};

class PossibleCards {
public:
	using CallbackType = std::function<void(const Card&)> ;
	static void forEach(CallbackType);
};

#endif // _CARD_
