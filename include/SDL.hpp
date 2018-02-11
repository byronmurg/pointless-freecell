#ifndef SDL_HPP
#define SDL_HPP

#include <memory>
#include <string>

#include <SDL.h>
#include <SDL_image.h>

#include <SDL/Error.hpp>
#include <SDL/Texture.hpp>
#include <SDL/Renderer.hpp>
#include <SDL/EventHandler.hpp>

#include <Point.hpp>

namespace SDL {

void init();
void exit();

void showMessage(const std::string&, const std::string&);
bool confirm(const std::string&, const std::string&);

void delay(unsigned int);

class Window {
public:
	Window(const char *, int, int);

	~Window();

	RendererPtr createRenderer();
	void refreshSize();

	int getWidth() const { return _width ; }
	int getHeight() const { return _height ; }

	Window& operator=(const Window&) = delete ;
	Window(const Window&) = delete ;
private:
	SDL_Window *_win ;
	int _width, _height ;
};

}

#endif // SDL_HPP
