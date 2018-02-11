#ifndef _SDL_TEXTURE_HPP
#define _SDL_TEXTURE_HPP

#include <memory>
#include <SDL.h>

#include <SDL/Error.hpp>

namespace SDL {

class Texture {
public:
	Texture(SDL_Texture *);
	~Texture();
	int getWidth() const ;
	int getHeight() const ;

	friend class Renderer ;

	Texture& operator=(const Texture&) = delete ;
	Texture(const Texture&) = delete ;
protected:
	SDL_Texture* getRawTexture(){ return _texture ; }
private:
	SDL_Texture *_texture ;
	int _width, _height ;
};

typedef std::shared_ptr<Texture> TexturePtr ;

}

#endif // _SDL_TEXTURE_HPP
