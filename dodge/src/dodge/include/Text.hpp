#pragma once

class Text
{
private:
	std::wstring text_;

	IDWriteTextFormat* format_;
	IDWriteTextLayout* layout_;

	float size_;
	float x_;
	float y_;

	bool show_;

	D2D1_COLOR_F color_;
public:
	Text() : text_(L""), show_(true), size_(1.0f), format_(0), layout_(0) {};
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
