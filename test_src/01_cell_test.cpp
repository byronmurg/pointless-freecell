
#include <iostream>
#include <Cell.hpp>

int main (){

	Cell cell1(Cell::Area::main, 1, 2) ;
	Cell cell2(Cell::Area::main, 3, 2) ;

	std::cout << bool(cell1 == cell2 ) << std::endl ;

}

