#pragma once

class BaseEnemy
{
private:
	D2D1_POINT_2F position_;
public:
	D2D1_POINT_2F get_position();

	const bool check_collision(const D2D1_RECT_F& position);

	void set_position(const float x, const float y);
	void draw(ID2D1HwndRenderTarget* d2d1_rt, ID2D1SolidColorBrush* d2d1_solidbrush);
};

