#pragma once
#include "Text.hpp"

struct dialog_options
{
	unsigned int limit;

	bool only_letters;
	bool only_numbers;
};

class Dialog
{
private:
	IDWriteFactory* dw_factory_;

	Text right_text_;
	Text left_text_;

	unsigned int input_len_;

	dialog_options options_;
public:
	Dialog();
	bool on_going;

	void init(IDWriteFactory* dw_factory);
	void destroy();

	void set_options(const dialog_options& options);
	void set_text(const std::wstring& input);

	void on_type(const short key);

	void draw(ID2D1HwndRenderTarget* d2d1_rt, ID2D1SolidColorBrush* d2d1_solidbrush);

	std::function<void(const std::wstring& input)> on_end;
};
