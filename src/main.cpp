
#include <SDL.hpp>
#include <Card.hpp>
#include <CardImageLoader.hpp>
#include <GameBoard.hpp>

//#ifndef NDEBUG
#include <iostream>
//#endif // NDEBUG

int main(){
	
	bool quit = false, win = false ;

	SDL::Window window("Freecell", 800, 600);

	auto renderer = window.createRenderer();
	renderer->setDrawColour(20, 100, 20, 0);
	renderer->clear();
	renderer->present();

	SDL::EventHandler eventHandler ;
	GameBoard gameBoard(renderer, window.getWidth(), window.getHeight());

	auto quitCallback = [&quit](){
			if ( SDL::confirm("quit", "are you sure you want to quit?") )
				quit = true ;
#ifndef NDEBUG
			std::cout << "Quit event recieved" << std::endl ;
#endif // NDEBUG
		};

	eventHandler.setQuitEventHandler(quitCallback);

	eventHandler.setKeyBoardEventHandler(
		[quitCallback](bool down, SDL::EventHandler::Key key){
			if ( down && key == SDL::EventHandler::Key::ESC ){
				quitCallback();
			}
		}
	);

	eventHandler.setResizeEventHandler(
		[&gameBoard]
		(int width, int height)
		{
			gameBoard.handleWindowResize(width, height);
		}	
	);

	eventHandler.setMouseDownEventHandler(
		[&gameBoard]
		(int width, int height)		
		{
			gameBoard.handleMouseDown(Point(width, height));
		}
	);

	eventHandler.setMouseUpEventHandler(
		[&gameBoard]
		(int width, int height)		
		{
			gameBoard.handleMouseUp(Point(width, height));
		}
	);

	eventHandler.setMouseDoubleClickEventHandler(
		[&gameBoard]
		(int width, int height)		
		{
			gameBoard.handleDoubleClick(Point(width, height));
		}
	);

	eventHandler.setMouseMoveEventHandler(
		[&gameBoard]
		(int width, int height)		
		{
			gameBoard.handleMouseMove(Point(width, height));
		}
	);

	gameBoard.setGameCompleteHandler(
		[&quit, &win]
		()
		{ win = quit = true ; }	
	);


	while (true){

		gameBoard.startGame();

		while ( ! quit ){
			eventHandler.pollEvents();
		
			renderer->clear();
		
			gameBoard.draw();
		
			renderer->present();
		
			SDL::delay(60);
		}

		if ( win ){
			SDL::showMessage("Game complete", "Congratulations you completed the game!");
			if ( ! SDL::confirm("play again", "Do you want to play again?") )
				break ;
			else
				quit = win = false ;
		} else {
			break ;
		}
		
	}


	SDL::exit();
	std::exit(EXIT_SUCCESS);
}

