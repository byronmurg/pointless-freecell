#include <CardImageLoader.hpp>

CardImageLoader::CardImageLoader
(RendererPtr renderer)
	: _cache()
	, _renderer(renderer)
{}


TexturePtr
CardImageLoader::getImage
(const Card& card){
	std::string fileName = "assets/images/" + card.getName() + ".png" ;
	if ( _cache.count(fileName) )
		return _cache[fileName];
	else
		return _cache[fileName] = _renderer->loadTexture(fileName);
}

