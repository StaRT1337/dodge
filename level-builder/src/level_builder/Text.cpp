#include "stdafx.hpp"
#include "Text.hpp"

#include "Utils.hpp"

bool Text::operator==(const Text& text)
{
	return layout == text.layout && format == text.format;
}

void Text::init(IDWriteFactory* factory)
{
	factory->CreateTextFormat(
		L"Arial",
		0,
		DWRITE_FONT_WEIGHT_NORMAL,
		DWRITE_FONT_STYLE_NORMAL,
		DWRITE_FONT_STRETCH_NORMAL,
		_size,
		L"en-us",
		&format
	);
}

void Text::destroy()
{
	Utils::safe_release(&format);
	Utils::safe_release(&layout);
}

void Text::hide()
{
	_show = false;
}

void Text::show()
{
	_show = true;
}

void Text::set_text(const std::wstring& string, IDWriteFactory* factory)
{	
	if (_text != string)
	{
		_text = string;

		Utils::safe_release(&layout);
		factory->CreateTextLayout(
			_text.data(),
			_text.length(),
			format,
			750,
			510,
			&layout
		);

		layout->SetFontSize(_size, DWRITE_TEXT_RANGE{ 0, _text.length() });
	}
}

void Text::set_color(const D2D1_COLOR_F& color)
{
	_color = color;
}

void Text::set_size(float size)
{
	_size = size;
	layout->SetFontSize(_size, DWRITE_TEXT_RANGE{ 0, _text.length() });
}

void Text::set_position(float x, float y)
{
	_x = x;
	_y = y;
}

const std::wstring Text::get_text()
{
	return _text;
}

DWRITE_TEXT_METRICS Text::get_metrics()
{
	DWRITE_TEXT_METRICS metrics;
	layout->GetMetrics(&metrics);

	return metrics;
}

void Text::draw(ID2D1HwndRenderTarget* d2d1_rt, ID2D1SolidColorBrush* d2d1_solidbrush)
{
	if (_show)
	{
		d2d1_solidbrush->SetColor(_color);

		d2d1_rt->DrawTextLayout(
			D2D1::Point2F(_x, _y),
			layout,
			d2d1_solidbrush
		);
	}
}

