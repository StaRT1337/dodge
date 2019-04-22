#pragma once
#include "Cube.hpp"

class Coin;
class Enemy;

class Player
{
private:
	D2D1_RECT_F player_rect_;
	D2D1_RECT_F stroke_rect_;

	const int check_collision(const D2D1_RECT_F& temp_stroke, std::vector<Cube>* cubes);
public:
	void on_type(std::vector<bool>* keys, std::vector<Cube>* cubes, std::vector<Coin>* coins, std::vector<Enemy>* enemies);

	D2D1_RECT_F  get_position();
	void set_position(const int x, const int y);

	void start(std::vector<Cube>* cubes, std::vector<Coin>* coins);

	void draw(ID2D1HwndRenderTarget* d2d1_rt, ID2D1SolidColorBrush* d2d1_solidbrush);
};
