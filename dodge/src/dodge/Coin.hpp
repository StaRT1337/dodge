#pragma once
#include "Cube.hpp"

class Coin
{
private:
	Cube cube_;
	D2D1_POINT_2F position_;
public:
	Cube get_cube();
	const D2D1_POINT_2F get_position();

	void set_cube(const Cube& cube);
	void draw(ID2D1HwndRenderTarget* d2d1_rt, ID2D1SolidColorBrush* d2d1_solidbrush);
};
