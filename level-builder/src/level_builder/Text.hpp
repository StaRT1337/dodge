#pragma once

class Text
{
private:
	std::wstring _text;

	IDWriteTextFormat* format;
	IDWriteTextLayout* layout;

	float _size;
	float _x;
	float _y;

	bool _show;

	D2D1_COLOR_F _color;
public:
	Text() : _text(L""), _show(true), _size(1.0f), format(0), layout(0) {};
	bool operator==(const Text& text);

	void init(IDWriteFactory* factory);
	void destroy();

	void hide();
	void show();

	void set_text(const std::wstring& string, IDWriteFactory* factory);
	void set_color(const D2D1_COLOR_F& color);

	void set_size(float size);
	void set_position(const float x, const float y);

	const std::wstring get_text();

	DWRITE_TEXT_METRICS get_metrics();

	void draw(ID2D1HwndRenderTarget* d2d1_rt, ID2D1SolidColorBrush* d2d1_solidbrush);
};
