#pragma once
#include "BaseEnemy.hpp"
#include "Player.hpp"

class Enemy
{
private:
	BaseEnemy base_;

	std::vector<D2D1_POINT_2F> points_;
	D2D1_POINT_2F position_;

	unsigned int pos_;
public:
	Enemy() : pos_(0), position_(D2D1::Point2F(-1, -1)) {};

	void add_point(const D2D1_POINT_2F& point);

	const bool check_collision(const D2D1_RECT_F& position);

	void clear();
	void draw(ID2D1HwndRenderTarget* d2d1_rt, ID2D1SolidColorBrush* d2d1_solidbrush);
};
