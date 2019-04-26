#pragma once
#include "BaseEnemy.hpp"

class SpinningEnemy
{
private:
	std::vector<BaseEnemy> bases_;
	D2D1_POINT_2F position_;
public:
	SpinningEnemy() : speed(-1.0f), spacing(-1.0f), amount(-1) {};

	float spacing;
	float speed;

	unsigned int amount;

	const D2D1_POINT_2F get_position();
	const bool check_collision(const D2D1_RECT_F& position);

	void set_enemies();
	void set_position(const float x, const float y);

	void draw(ID2D1HwndRenderTarget* d2d1_rt, ID2D1SolidColorBrush* d2d1_solidbrush);
};
