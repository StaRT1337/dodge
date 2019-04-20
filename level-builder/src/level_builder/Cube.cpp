#include "stdafx.hpp"
#include "Cube.hpp"

#include "Utils.hpp"

void Cube::set_size(std::uintptr_t width, std::uint32_t height)
{
	_width = width;
	_height = height;
}

void Cube::set_position(int x, int y)
{
	_x = x;
	_y = y;
}

void Cube::set_pos(const std::uint32_t pos)
{
	_pos = pos;
}

void Cube::draw(ID2D1RenderTarget* render_target, ID2D1SolidColorBrush* solid_brush)
{
	D2D1_COLOR_F color;

	switch (_type)
	{
	case cube_type::BORDER_CUBE:
		color = Utils::create_d2d1_color(180, 181, 254, 255);
		break;
	case cube_type::REGULAR_CUBE:
		color = calculate_color();
		break;
	case cube_type::SPAWN_CUBE:
		color = Utils::create_d2d1_color(181, 254, 180, 255);
		break;
	}

	if (!Utils::cmp_d2d1_color(color, solid_brush->GetColor()))
	{
		solid_brush->SetColor(color);
	}

	render_target->FillRectangle(
		D2D1::RectF(_x, _y, _x + _width, _y + _height),
		solid_brush
	);
}

void Cube::set_type(const cube_type& type)
{
	_type = type;
}

void Cube::update(std::vector<Cube>* cubes)
{
	cubes->at(_pos) = *this;
}

const std::uint32_t Cube::get_vec_pos()
{
	return _pos;
}

const D2D1_POINT_2F Cube::get_position()
{
	return D2D1::Point2F(_x, _y);
}

const cube_type Cube::get_type()
{
	return _type;
}

const D2D1_COLOR_F Cube::calculate_color()
{
	D2D1_COLOR_F color;

	bool white;

	if ((_y / 10) % 2 == 0)
	{
		white = (_x / 10) % 2 == 0;
	}
	else
	{
		white = (_x / 10) % 2 != 0;
	}

	white ? color = Utils::create_d2d1_color(255, 255, 255, 255) : color = Utils::create_d2d1_color(230, 230, 255, 255);
	return color;
}
