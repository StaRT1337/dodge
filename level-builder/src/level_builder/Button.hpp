#pragma once
#include "Text.hpp"

enum class mouse_type : std::uint32_t
{
	NONE,
	LBUTTON
};

class Button
{
private:
	Text _text;

	bool _show;
	bool _fixedsize;

	D2D1_RECT_F button_rect;
	D2D1_RECT_F stroke_rect;

	D2D1_COLOR_F _strokecolor;
	D2D1_COLOR_F _buttoncolor;

	bool check_collision(const POINT& mouse_position);
public:
	Button();
	bool operator==(const Button& op);

	void init(IDWriteFactory* dw_factory);
	void destroy();

	void set_text(const std::wstring& text, IDWriteFactory* dw_factory);
	void set_text_color(const D2D1_COLOR_F& color);

	void set_text_size(const float size);
	void set_size(const int x, const int y);

	void set_position(const int x, const int y);

	void set_button_color(const D2D1_COLOR_F& color);

	const std::wstring get_text();

	void check_click(const POINT& mouse_position, const mouse_type& button);
	void check_hover(const POINT& mouse_position);

	void fixed_size(const bool state);

	void show();
	void hide();

	void draw(ID2D1HwndRenderTarget* d2d1_rt, ID2D1SolidColorBrush* d2d1_solidbrush);

	std::function<void(Button* sender, const mouse_type& button)> on_click;
	std::function<void(Button* sender)> on_hover;
};
