#include "stdafx.hpp"

#include "include/Coin.hpp"
#include "include/Utils.hpp"

Cube Coin::get_cube()
{
	return cube_;
}

const D2D1_POINT_2F Coin::get_position()
{
	return position_;
}

void Coin::check_collision(Player* player, const D2D1_RECT_F& position)
{
	auto delta_x = position_.x - std::max(position.left, std::min(position_.x, position.left + 23.0f));
	auto delta_y = position_.y - std::max(position.top, std::min(position_.y, position.top + 23.0f));

	auto collision =  (delta_x * delta_x + delta_y * delta_y) < (8.0f * 8.0f);

	if (collision)
	{
		collected = true;
	}
}

void Coin::set_cube(const Cube& cube)
{
	cube_ = cube;
	auto position = cube_.get_position();

	position_.x = position.x + 15;
	position_.y = position.y + 15;
}

void Coin::draw(ID2D1HwndRenderTarget* d2d1_rt, ID2D1SolidColorBrush* d2d1_solidbrush)
{
	if (collected) return;

	d2d1_solidbrush->SetColor(Utils::create_d2d1_color(0, 0, 0, 255));
	d2d1_rt->FillEllipse(
		D2D1::Ellipse(
			position_,
			8.0f,
			8.0f
		),
		d2d1_solidbrush
	);

	d2d1_solidbrush->SetColor(Utils::create_d2d1_color(255, 255, 0, 255));
	d2d1_rt->FillEllipse(
		D2D1::Ellipse(
			position_,
			4.0f,
			4.0f
		),
		d2d1_solidbrush
	);
}
