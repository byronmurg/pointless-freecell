#ifndef _VISUALIZER_HPP_
#define _VISUALIZER_HPP_

#include <Card.hpp>
#include <AABB.hpp>

class Visualizer {
	drawCard(const Card&, const AABB&) = 0 ;
	showMessage(const std::string& title, const std::string& message) = 0 ;
	~Visualizer(){};
}

#endif // _VISUALIZER_HPP_
