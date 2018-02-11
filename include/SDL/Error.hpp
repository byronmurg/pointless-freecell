#ifndef _SDL_ERROR_HPP_
#define _SDL_ERROR_HPP_

#include <exception>
#include <string>

#include <SDL.h>

namespace SDL {

class Error : public std::exception {
public:
	Error(const std::string& msgPrefix = "")
		: _errMsg(msgPrefix +" : "+ SDL_GetError())
	{}
	const char *what() const noexcept { return _errMsg.c_str() ; }
private:
	std::string _errMsg ;
};

}

#endif // _SDL_ERROR_HPP_
