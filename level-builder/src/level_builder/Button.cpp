#include "stdafx.hpp"
#include "Button.hpp"

#include "Utils.hpp"

Button::Button() : _show(true), _fixedsize(false), _buttoncolor(Utils::create_d2d1_color(255, 255, 255, 255)), _strokecolor(Utils::create_d2d1_color(0, 0, 0, 255))
{
}

bool Button::operator==(const Button& op)
{
	return _text == op._text;
}

void Button::init(IDWriteFactory* dw_factory)
{
	_text.init(dw_factory);

	_text.set_color(Utils::create_d2d1_color(0, 0, 0, 255));
}

void Button::destroy()
{
	_text.destroy();
}

void Button::set_text(const std::wstring& text, IDWriteFactory* dw_factory)
{
	_text.set_text(text, dw_factory);

	if (!_fixedsize)
	{
		auto metrics = _text.get_metrics();

		stroke_rect.right = stroke_rect.left + metrics.width + 20;
		stroke_rect.bottom = stroke_rect.top + metrics.height + 15;
		button_rect.right = button_rect.left + metrics.width + 10;
		button_rect.bottom = button_rect.top + metrics.height + 5;
	}
}

void Button::set_size(const int x, const int y)
{
	if (_fixedsize)
	{
		stroke_rect.right = stroke_rect.left + x;
		stroke_rect.bottom = stroke_rect.top + y;
		button_rect.right = button_rect.left + x - 10;
		button_rect.bottom = button_rect.top + y - 10;

		auto metrics = _text.get_metrics();

		_text.set_position(stroke_rect.left + (x - metrics.width) / 2, stroke_rect.top + (y - metrics.height) / 2);
	}
}

void Button::set_text_color(const D2D1_COLOR_F& color)
{
	_text.set_color(color);
}

void Button::set_text_size(const float size)
{
	_text.set_size(size);

	if (!_fixedsize)
	{
		auto metrics = _text.get_metrics();

		stroke_rect.right = stroke_rect.left + metrics.width + 20;
		stroke_rect.bottom = stroke_rect.top + metrics.height + 15;
		button_rect.right = button_rect.left + metrics.width + 10;
		button_rect.bottom = button_rect.top + metrics.height + 5;
	}
}

void Button::set_position(const int x, const int y)
{
	stroke_rect.left = x;
	stroke_rect.top = y;

	button_rect.left = x + 5;
	button_rect.top = y + 5;

	if (!_fixedsize)
	{
		_text.set_position(x + 10, y + 7);
	}
}

void Button::set_button_color(const D2D1_COLOR_F& color)
{
	_buttoncolor = color;
}

const std::wstring Button::get_text()
{
	return _text.get_text();
}

void Button::check_click(const POINT& mouse_position, const mouse_type& button)
{
	if (button == mouse_type::NONE) return;

	if (check_collision(mouse_position) && on_click)
	{
		on_click(this, button);
	}
}

void Button::check_hover(const POINT& mouse_position)
{
	if (check_collision(mouse_position) && on_hover)
	{
		on_hover(this);
	}
}

void Button::fixed_size(const bool state)
{
	_fixedsize = state;
}

void Button::show()
{
	_show = true;
}

void Button::hide()
{
	_show = false;
}

void Button::draw(ID2D1HwndRenderTarget* d2d1_rt, ID2D1SolidColorBrush* d2d1_solidbrush)
{
	if (!_show) return;

	auto metrics = _text.get_metrics();

	d2d1_solidbrush->SetColor(_strokecolor);
	d2d1_rt->FillRectangle(
		D2D1::RectF(stroke_rect.left, stroke_rect.top, stroke_rect.right, stroke_rect.bottom),
		d2d1_solidbrush
	);

	d2d1_solidbrush->SetColor(_buttoncolor);
	d2d1_rt->FillRectangle(
		D2D1::RectF(button_rect.left, button_rect.top, button_rect.right, button_rect.bottom),
		d2d1_solidbrush
	);

	_text.draw(d2d1_rt, d2d1_solidbrush);
}

bool Button::check_collision(const POINT& mouse_position)
{
	auto x = static_cast<int>(mouse_position.x);
	auto y = static_cast<int>(mouse_position.y);

	if ((x < button_rect.right && x - button_rect.left > 0) && (y < button_rect.bottom && y - button_rect.top > 0))
	{
		return true;
	}

	return false;
}
