#ifndef _EVENT_HANDLER_
#define _EVENT_HANDLER_

#include <functional>

#include <SDL.h>

namespace SDL {

class EventHandler {
public:
	enum Key {
		ESC, LEFT, UP, DOWN, RIGHT,
		UNKNOWN
	};

	using KeyBoardEventHandler = std::function<void(bool, Key)> ;
	using QuitEventHandler = std::function<void(void)> ;
	using ResizeEventHandler = std::function<void(int, int)> ;
	using MouseMoveEventHandler = std::function<void(int, int)> ;
	using MouseDownEventHandler = std::function<void(int, int)> ;
	using MouseUpEventHandler = std::function<void(int, int)> ;
	using MouseDoubleClickEventHandler = std::function<void(int, int)> ;

	EventHandler();

	void setKeyBoardEventHandler(KeyBoardEventHandler kbeh){
		_keyBoardEventHandler = kbeh ;
	}

	void setQuitEventHandler(QuitEventHandler qeh){
		_quitHandler = qeh ;
	}

	void setResizeEventHandler(ResizeEventHandler reeh){
		_resizeEventHandler = reeh ;
	}

	void setMouseMoveEventHandler(MouseMoveEventHandler mmeh){
		_mouseMoveEventHandler = mmeh ;
	}

	void setMouseDownEventHandler(MouseDownEventHandler mdeh){
		_mouseDownEventHandler = mdeh ;
	}

	void setMouseUpEventHandler(MouseUpEventHandler mueh){
		_mouseUpEventHandler = mueh ;
	}

	void setMouseDoubleClickEventHandler(MouseDoubleClickEventHandler mdeh){
		_mouseDoubleClickEventHandler = mdeh ;
	}


	void pollEvents();

private:
	void _processEvent(const SDL_Event&);
	Key _convertKeyToNativeType(const SDL_Keycode keyCode);
	void _processMouseMove(const SDL_Event&);
	void _processMouseStateChange(const SDL_Event&);
	void _processKeyBoardEvent(const SDL_Event&);
	void _processWindowEvent(const SDL_Event&);

	MouseMoveEventHandler _mouseMoveEventHandler ;
	MouseDownEventHandler _mouseDownEventHandler ;
	MouseUpEventHandler _mouseUpEventHandler ;
	MouseDoubleClickEventHandler _mouseDoubleClickEventHandler ;
	QuitEventHandler _quitHandler ;
	KeyBoardEventHandler _keyBoardEventHandler ;
	ResizeEventHandler _resizeEventHandler ;
};

}

#endif // _EVENT_HANDLER_
