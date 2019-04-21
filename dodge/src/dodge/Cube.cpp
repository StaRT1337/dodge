#include "stdafx.hpp"

#include "include/Cube.hpp"
#include "include/Utils.hpp"

void Cube::set_size(std::uintptr_t width, std::uint32_t height)
{
	width_ = width;
	height_ = height;
}

void Cube::set_position(int x, int y)
{
	x_ = x;
	y_ = y;
}

void Cube::set_pos(const std::uint32_t pos)
{
	pos_ = pos;
}

void Cube::draw(ID2D1RenderTarget* render_target, ID2D1SolidColorBrush* solid_brush)
{
	D2D1_COLOR_F color;

	switch (type_)
	{
	case cube_type::BORDER_CUBE:
		color = Utils::create_d2d1_color(180, 181, 254, 255);
		break;
	case cube_type::REGULAR_CUBE:
		color = calculate_color();
		break;
	case cube_type::SPAWN_CUBE:
		color = Utils::create_d2d1_color(81, 179, 255, 255);
		break;
	case cube_type::END_CUBE:
		color = Utils::create_d2d1_color(181, 254, 180, 255);
		break;
	}

	if (!Utils::cmp_d2d1_color(color, solid_brush->GetColor()))
	{
		solid_brush->SetColor(color);
	}

	render_target->FillRectangle(
		D2D1::RectF(x_, y_, x_ + width_, y_ + height_),
		solid_brush
	);
}

void Cube::set_type(const cube_type& type)
{
	type_ = type;
}

void Cube::update(std::vector<Cube>* cubes)
{
	cubes->at(pos_) = *this;
}

const std::uint32_t Cube::get_vec_pos()
{
	return pos_;
}

const D2D1_POINT_2F Cube::get_position()
{
	return D2D1::Point2F(x_, y_);
}

const cube_type Cube::get_type()
{
	return type_;
}

const D2D1_COLOR_F Cube::calculate_color()
{
	D2D1_COLOR_F color;

	bool white;

	if ((y_ / 10) % 2 == 0)
	{
		white = (x_ / 10) % 2 == 0;
	}
	else
	{
		white = (x_ / 10) % 2 != 0;
	}

	white ? color = Utils::create_d2d1_color(255, 255, 255, 255) : color = Utils::create_d2d1_color(230, 230, 255, 255);
	return color;
}
