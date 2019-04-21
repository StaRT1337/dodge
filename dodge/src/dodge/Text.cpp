#include "stdafx.hpp"

#include "include/Text.hpp"
#include "include/Utils.hpp"

bool Text::operator==(const Text& text)
{
	return layout_ == text.layout_ && format_ == text.format_;
}

void Text::init(IDWriteFactory* factory)
{
	factory->CreateTextFormat(
		L"Arial",
		0,
		DWRITE_FONT_WEIGHT_NORMAL,
		DWRITE_FONT_STYLE_NORMAL,
		DWRITE_FONT_STRETCH_NORMAL,
		size_,
		L"en-us",
		&format_
	);
}

void Text::destroy()
{
	Utils::safe_release(&format_);
	Utils::safe_release(&layout_);
}

void Text::hide()
{
	show_ = false;
}

void Text::show()
{
	show_ = true;
}

void Text::set_text(const std::wstring& string, IDWriteFactory* factory)
{
	if (text_ != string)
	{
		text_ = string;

		Utils::safe_release(&layout_);
		factory->CreateTextLayout(
			text_.data(),
			text_.length(),
			format_,
			750,
			510,
			&layout_
		);

		layout_->SetFontSize(size_, DWRITE_TEXT_RANGE{ 0, text_.length() });
	}
}

void Text::set_color(const D2D1_COLOR_F& color)
{
	color_ = color;
}

void Text::set_size(float size)
{
	size_ = size;
	layout_->SetFontSize(size_, DWRITE_TEXT_RANGE{ 0, text_.length() });
}

void Text::set_position(float x, float y)
{
	x_ = x;
	y_ = y;
}

const std::wstring Text::get_text()
{
	return text_;
}

DWRITE_TEXT_METRICS Text::get_metrics()
{
	DWRITE_TEXT_METRICS metrics;
	layout_->GetMetrics(&metrics);

	return metrics;
}

void Text::draw(ID2D1HwndRenderTarget* d2d1_rt, ID2D1SolidColorBrush* d2d1_solidbrush)
{
	if (show_)
	{
		d2d1_solidbrush->SetColor(color_);

		d2d1_rt->DrawTextLayout(
			D2D1::Point2F(x_, y_),
			layout_,
			d2d1_solidbrush
		);
	}
}

