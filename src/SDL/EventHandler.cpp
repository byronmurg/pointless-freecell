#include <SDL/EventHandler.hpp>

namespace SDL {

EventHandler::EventHandler()
	: _mouseMoveEventHandler(nullptr)
	, _mouseDownEventHandler(nullptr)
	, _mouseUpEventHandler(nullptr)
	, _mouseDoubleClickEventHandler(nullptr)
	, _quitHandler(nullptr)
	, _keyBoardEventHandler(nullptr)
	, _resizeEventHandler(nullptr)
{}

void EventHandler::pollEvents(){
	SDL_Event e ;

	while ( SDL_PollEvent(&e) )
		_processEvent(e);
}

void EventHandler::_processEvent(const SDL_Event &e){
	switch ( e.type ){
		case SDL_QUIT :
			_quitHandler();
			break ;
		case SDL_WINDOWEVENT :
			_processWindowEvent(e);
			break ;
		case SDL_MOUSEBUTTONDOWN :
		case SDL_MOUSEBUTTONUP :
			_processMouseStateChange(e);
			break ;
		case SDL_MOUSEMOTION :
			_processMouseMove(e);
		case SDL_KEYDOWN :
		case SDL_KEYUP :
			_processKeyBoardEvent(e);
			break ;
	}
}

void EventHandler::_processMouseStateChange(const SDL_Event &e){
	auto *mbe = (SDL_MouseButtonEvent *) &e ;
	if ( ( mbe->clicks == 2 ) && ( mbe->state == SDL_RELEASED ) ){
		if ( _mouseDoubleClickEventHandler ) _mouseDoubleClickEventHandler(mbe->x, mbe->y);
	} else if ( mbe->state == SDL_PRESSED ){
		if ( _mouseDownEventHandler ) _mouseDownEventHandler(mbe->x, mbe->y);
	} else {
		if (_mouseUpEventHandler) _mouseUpEventHandler(mbe->x, mbe->y);
	}
}

void EventHandler::_processMouseMove(const SDL_Event &e){
	auto *mme = (SDL_MouseMotionEvent *) &e ;
	if ( _mouseMoveEventHandler )
		_mouseMoveEventHandler(mme->x, mme->y);
}

void EventHandler::_processWindowEvent(const SDL_Event &e){
	switch ( e.window.event ){
//		case SDL_WINDOWEVENT_RESIZED:
		case SDL_WINDOWEVENT_SIZE_CHANGED:
			  if ( _resizeEventHandler )
				  _resizeEventHandler(e.window.data1, e.window.data2);
			  break ;
	}
}

void EventHandler::_processKeyBoardEvent(const SDL_Event &e){
	SDL_KeyboardEvent *kbe = (SDL_KeyboardEvent *) &e ;
	auto key = _convertKeyToNativeType(kbe->keysym.sym);
	bool pressed = kbe->state == SDL_PRESSED ;
	_keyBoardEventHandler(pressed, key);
}

EventHandler::Key EventHandler::_convertKeyToNativeType(const SDL_Keycode keySym){
	switch ( keySym ){
		case SDLK_ESCAPE:
			return ESC ;
		case SDLK_w :
			return UP ;
		case SDLK_a :
			return LEFT ;
		case SDLK_s :
			return DOWN ;
		case SDLK_d :
			return RIGHT ;
	}
	return UNKNOWN ;
}

}
