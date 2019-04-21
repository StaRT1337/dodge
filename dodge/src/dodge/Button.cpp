#include "stdafx.hpp"
#include "Button.hpp"

#include "Utils.hpp"

Button::Button() : show_(true), fixedsize_(false), buttoncolor_(Utils::create_d2d1_color(255, 255, 255, 255)), strokecolor_(Utils::create_d2d1_color(0, 0, 0, 255))
{
}

bool Button::operator==(const Button& op)
{
	return text_ == op.text_;
}

void Button::init(IDWriteFactory* dw_factory)
{
	text_.init(dw_factory);

	text_.set_color(Utils::create_d2d1_color(0, 0, 0, 255));
}

void Button::destroy()
{
	text_.destroy();
}

void Button::set_text(const std::wstring& text, IDWriteFactory* dw_factory)
{
	text_.set_text(text, dw_factory);

	if (!fixedsize_)
	{
		auto metrics = text_.get_metrics();

		stroke_rect_.right = stroke_rect_.left + metrics.width + 20;
		stroke_rect_.bottom = stroke_rect_.top + metrics.height + 15;
		button_rect_.right = button_rect_.left + metrics.width + 10;
		button_rect_.bottom = button_rect_.top + metrics.height + 5;
	}
}

void Button::set_size(const int x, const int y)
{
	if (fixedsize_)
	{
		stroke_rect_.right = stroke_rect_.left + x;
		stroke_rect_.bottom = stroke_rect_.top + y;
		button_rect_.right = button_rect_.left + x - 10;
		button_rect_.bottom = button_rect_.top + y - 10;

		auto metrics = text_.get_metrics();

		text_.set_position(stroke_rect_.left + (x - metrics.width) / 2, stroke_rect_.top + (y - metrics.height) / 2);
	}
}

void Button::set_text_color(const D2D1_COLOR_F& color)
{
	text_.set_color(color);
}

void Button::set_text_size(const float size)
{
	text_.set_size(size);

	if (!fixedsize_)
	{
		auto metrics = text_.get_metrics();

		stroke_rect_.right = stroke_rect_.left + metrics.width + 20;
		stroke_rect_.bottom = stroke_rect_.top + metrics.height + 15;
		button_rect_.right = button_rect_.left + metrics.width + 10;
		button_rect_.bottom = button_rect_.top + metrics.height + 5;
	}
}

void Button::set_position(const int x, const int y)
{
	stroke_rect_.left = x;
	stroke_rect_.top = y;

	button_rect_.left = x + 5;
	button_rect_.top = y + 5;

	if (!fixedsize_)
	{
		text_.set_position(x + 10, y + 7);
	}
}

void Button::set_button_color(const D2D1_COLOR_F& color)
{
	buttoncolor_ = color;
}

const std::wstring Button::get_text()
{
	return text_.get_text();
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
	fixedsize_ = state;
}

void Button::show()
{
	show_ = true;
}

void Button::hide()
{
	show_ = false;
}

void Button::draw(ID2D1HwndRenderTarget* d2d1_rt, ID2D1SolidColorBrush* d2d1_solidbrush)
{
	if (!show_) return;

	auto metrics = text_.get_metrics();

	d2d1_solidbrush->SetColor(strokecolor_);
	d2d1_rt->FillRectangle(
		D2D1::RectF(stroke_rect_.left, stroke_rect_.top, stroke_rect_.right, stroke_rect_.bottom),
		d2d1_solidbrush
	);

	d2d1_solidbrush->SetColor(buttoncolor_);
	d2d1_rt->FillRectangle(
		D2D1::RectF(button_rect_.left, button_rect_.top, button_rect_.right, button_rect_.bottom),
		d2d1_solidbrush
	);

	text_.draw(d2d1_rt, d2d1_solidbrush);
}

bool Button::check_collision(const POINT& mouse_position)
{
	auto x = static_cast<int>(mouse_position.x);
	auto y = static_cast<int>(mouse_position.y);

	if ((x < button_rect_.right && x - button_rect_.left > 0) && (y < button_rect_.bottom && y - button_rect_.top > 0))
	{
		return true;
	}

	return false;
}
