#include <SDL/Renderer.hpp>

namespace SDL {

Renderer::Renderer(SDL_Renderer *renderer)
	: _renderer(renderer)
{
	if ( ! _renderer )
		throw Error("Failed to initialize renderer");
	setDrawColour(0xFF, 0xFF, 0xFF, 0xFF);
}

void
Renderer::setDrawColour
(int r, int g, int b, int a)
{
	SDL_SetRenderDrawColor(_renderer, r, g, b, a);
}

TexturePtr Renderer::loadTexture(const std::string& fileName){
	SDL_Surface *surface = IMG_Load(fileName.c_str());

	SDL_Texture *texture = SDL_CreateTextureFromSurface(_renderer, surface);
	if ( ! texture )
		throw Error("creating texture "+ fileName);

	SDL_FreeSurface(surface);

	return std::make_shared<Texture>(texture);
}

void Renderer::copy(const TexturePtr& texture){
	SDL_RenderCopy(_renderer, texture->getRawTexture(), NULL, NULL);
}

void Renderer::copy(const TexturePtr& texture, const Rect& dstRect){
	SDL_RenderCopy(
			  _renderer
			, texture->getRawTexture()
			, NULL
			, dstRect.getPointr()
		);
}

void Renderer::copy(const TexturePtr& texture, const Rect& dstRect, const Rect& srcRect){
	SDL_RenderCopy(
			  _renderer
			, texture->getRawTexture()
			, srcRect.getPointr()
			, dstRect.getPointr()
		);
}

Renderer::~Renderer(){
	SDL_DestroyRenderer(_renderer);
}

void Renderer::clear(){
	SDL_RenderClear(_renderer);
}

void Renderer::present(){
	SDL_RenderPresent(_renderer);
}

}
