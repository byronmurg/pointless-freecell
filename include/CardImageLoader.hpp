#ifndef _CARD_IMAGE_LOADER_
#define _CARD_IMAGE_LOADER_

#include <map>
#include <memory>
#include <string>

#include <SDL/Texture.hpp>
#include <SDL/Renderer.hpp>
#include <Card.hpp>

using namespace SDL ;

class CardImageLoader {
public:
	using ImageMap = std::map<std::string, TexturePtr> ;

	CardImageLoader(RendererPtr);
	CardImageLoader(const CardImageLoader&) = default ;
	
	TexturePtr getImage(const Card&);
private:
	ImageMap _cache ;
	RendererPtr _renderer ;
};

typedef std::shared_ptr<CardImageLoader> CardImageLoaderPtr ;

#endif // _CARD_IMAGE_LOADER_
