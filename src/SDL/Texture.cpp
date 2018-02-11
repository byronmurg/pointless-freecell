#include <SDL/Texture.hpp>

namespace SDL {

Texture::Texture(SDL_Texture *texture)
	: _texture(texture)
	, _width(0)
	, _height(0)
{
	if ( ! _texture )
		throw Error("initializing texture");

	if ( SDL_QueryTexture(_texture, NULL, NULL, &_width, &_height) )
		throw Error("querying texture paramaters");
}

Texture::~Texture(){
	SDL_DestroyTexture(_texture);
}

int Texture::getWidth() const {
	return _width ;
}

int Texture::getHeight() const {
	return _height ;
}

}
