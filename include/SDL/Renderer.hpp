#ifndef _SDL_RENDERER_HPP_
#define _SDL_RENDERER_HPP_

#include <memory>

#include <SDL.h>
#include <SDL_image.h>

#include <SDL/Error.hpp>
#include <SDL/Texture.hpp>
#include <SDL/Rect.hpp>

namespace SDL {

class Renderer {
public:
	friend class Window ;
	~Renderer();

	void setDrawColour(int, int, int, int);
	TexturePtr loadTexture(const std::string& fileName);
	void clear();
	void present();
	void copy(const TexturePtr&);
	void copy(const TexturePtr&, const Rect&);
	void copy(const TexturePtr&, const Rect&, const Rect&);

	Renderer& operator=(const Renderer&) = delete ;
	Renderer(const Renderer&) = delete ;
protected:
	Renderer(SDL_Renderer *renderer);
private:
	SDL_Renderer *_renderer ;
};

typedef std::shared_ptr<Renderer> RendererPtr ;

}

#endif // _SDL_RENDERER_HPP_
