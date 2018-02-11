#include <SDL.hpp>

namespace SDL {


void init(){
	SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO);
	IMG_Init(IMG_INIT_PNG);
}

void exit(){
	IMG_Quit();
	SDL_Quit();
}

void
showMessage
(const std::string &title, const std::string &text)
{
	SDL_ShowSimpleMessageBox(
			SDL_MESSAGEBOX_ERROR,
			title.c_str(),
			text.c_str(),
			NULL
			);
}

bool
confirm
(const std::string &title, const std::string &text)
{
	const SDL_MessageBoxButtonData buttons[] = {
		{ SDL_MESSAGEBOX_BUTTON_ESCAPEKEY_DEFAULT, 0, "no" },
		{ SDL_MESSAGEBOX_BUTTON_RETURNKEY_DEFAULT, 1, "yes" }
	};

	const SDL_MessageBoxData messageBoxData = {
		SDL_MESSAGEBOX_INFORMATION,
		NULL,
		title.c_str(),
		text.c_str(),
		2,
		buttons,
		NULL
	};

	int buttonReturn(0);
	SDL_ShowMessageBox(&messageBoxData, &buttonReturn);

	return bool(buttonReturn);
}

void delay(unsigned int milliseconds){
	SDL_Delay(milliseconds);
}


Window::Window(const char *title, int width, int height)
	: _win(SDL_CreateWindow(
		  title
		, SDL_WINDOWPOS_UNDEFINED
		, SDL_WINDOWPOS_UNDEFINED
		, width
		, height
		//, SDL_WINDOW_FULLSCREEN_DESKTOP
		, SDL_WINDOW_RESIZABLE
		))
	, _width()
	, _height()
{
	if ( ! _win )
		throw Error("Failed to initialize window");

	refreshSize();
}

void Window::refreshSize(){
	SDL_GetWindowSize(_win, &_width, &_height);
}

RendererPtr Window::createRenderer(){
	auto r = new Renderer(SDL_CreateRenderer(_win, -1, SDL_RENDERER_ACCELERATED));
	return RendererPtr(r);
}

Window::~Window(){
	SDL_DestroyWindow(_win);
}

}
