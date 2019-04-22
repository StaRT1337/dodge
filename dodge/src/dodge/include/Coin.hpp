#pragma once
#include "Cube.hpp"

class Player;

class Coin
{
private:
	Cube cube_;
	D2D1_POINT_2F position_;

public:
	Coin() : collected(false) {};

	bool collected;

	Cube get_cube();

	const D2D1_POINT_2F get_position();

	void check_collision(Player* player, const D2D1_RECT_F& position);

	void set_cube(const Cube& cube);
	void draw(ID2D1HwndRenderTarget* d2d1_rt, ID2D1SolidColorBrush* d2d1_solidbrush);
};
