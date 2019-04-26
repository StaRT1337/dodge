#include "stdafx.hpp"

#include "include/BaseEnemy.hpp"
#include "include/Utils.hpp"

D2D1_POINT_2F BaseEnemy::get_position()
{
	return position_;
}

const bool BaseEnemy::check_collision(const D2D1_RECT_F & position)
{
	auto delta_x = position_.x - std::max(position.left, std::min(position_.x, position.left + 23.0f));
	auto delta_y = position_.y - std::max(position.top, std::min(position_.y, position.top + 23.0f));

	return (delta_x * delta_x + delta_y * delta_y) < (8.0f * 8.0f);
}

void BaseEnemy::set_position(const float x, const float y)
{
	position_.x = x;
	position_.y = y;
}

void BaseEnemy::draw(ID2D1HwndRenderTarget* d2d1_rt, ID2D1SolidColorBrush* d2d1_solidbrush)
{
	d2d1_solidbrush->SetColor(Utils::create_d2d1_color(0, 0, 0, 255));
	d2d1_rt->FillEllipse(
		D2D1::Ellipse(
			position_,
			8.0f,
			8.0f
		),
		d2d1_solidbrush
	);

	d2d1_solidbrush->SetColor(Utils::create_d2d1_color(0, 0, 255, 255));
	d2d1_rt->FillEllipse(
		D2D1::Ellipse(
			position_,
			4.0f,
			4.0f
		),
		d2d1_solidbrush
	);
}
