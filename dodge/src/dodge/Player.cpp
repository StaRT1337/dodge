#include "stdafx.hpp"
#include "Player.hpp"

#include "Utils.hpp"

constexpr int w = 0x57;
constexpr int a = 0x41;
constexpr int s = 0x53;
constexpr int d = 0x44;

constexpr int speed = 4;

void Player::on_type(std::vector<bool>* keys, std::vector<Cube>* cubes)
{
	/*
		WARNING: What you are about to see, is the most shitcode ever created,
		read at your own risk.
	*/

	D2D1_RECT_F temp_stroke = stroke_rect_;
	D2D1_RECT_F temp_player = player_rect_;


	bool up = false;
	bool down = false;
	bool right = false;
	bool left = false;

	if (keys->at(w))
	{
		up = true;
	}
	if (keys->at(s))
	{
		down = true;
	}
	if (keys->at(d))
	{
		right = true;
	}
	if (keys->at(a))
	{
		left = true;
	}

	if (up)
	{
		temp_stroke.top -= speed;
		temp_player.top -= speed;
	}
	if (down)
	{
		temp_stroke.top += speed;
		temp_player.top += speed;
	}
	if (right)
	{
		temp_stroke.left += speed;
		temp_player.left += speed;
	}
	if (left)
	{
		temp_stroke.left -= speed;
		temp_player.left -= speed;
	}

	if (check_collision(temp_stroke, cubes))
	{
		return;
	}
	else
	{
		stroke_rect_ = temp_stroke;
		player_rect_ = temp_player;
	}
}

void Player::set_position(const int x, const int y)
{
	stroke_rect_.left = x;
	stroke_rect_.top = y;

	player_rect_.left = x + 4;
	player_rect_.top = y + 4;
}

void Player::draw(ID2D1HwndRenderTarget* d2d1_rt, ID2D1SolidColorBrush* d2d1_solidbrush)
{
	stroke_rect_.right = stroke_rect_.left + 23;
	stroke_rect_.bottom = stroke_rect_.top + 23;

	player_rect_.right = player_rect_.left + 15;
	player_rect_.bottom = player_rect_.top + 15;

	d2d1_solidbrush->SetColor(Utils::create_d2d1_color(0, 0, 0, 255));
	d2d1_rt->FillRectangle(
		stroke_rect_,
		d2d1_solidbrush
	);

	d2d1_solidbrush->SetColor(Utils::create_d2d1_color(255, 0, 0, 255));
	d2d1_rt->FillRectangle(
		player_rect_,
		d2d1_solidbrush
	);
}

const bool Player::check_collision(const D2D1_RECT_F& temp_stroke, std::vector<Cube>* cubes)
{
	std::vector<std::pair<float, float>> points;

	points.emplace_back(temp_stroke.left, temp_stroke.top);
	points.emplace_back(temp_stroke.left + (23 / 2), temp_stroke.top);
	points.emplace_back(temp_stroke.left + 23, temp_stroke.top);

	points.emplace_back(temp_stroke.left, temp_stroke.top + 23);
	points.emplace_back(temp_stroke.left + (23 / 2), temp_stroke.top + 23);
	points.emplace_back(temp_stroke.left + 23, temp_stroke.top + 23);

	for (const auto& pair : points)
	{
		if (Utils::get_cube(POINT{ static_cast<long>(pair.first), static_cast<long>(pair.second) }, cubes).get_type() == cube_type::BORDER_CUBE || pair.first >= 750 || pair.first <= 0)
		{
			return true;
		}
	}

	return false;
}
