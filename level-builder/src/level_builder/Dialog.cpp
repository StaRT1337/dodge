#include "stdafx.hpp"

#include "include/Dialog.hpp"
#include "include/Utils.hpp"

Dialog::Dialog() : on_going(false), dw_factory_(0)
{
	right_text_.hide();
	right_text_.set_color(Utils::create_d2d1_color(0, 0, 0, 255));

	left_text_.hide();
	left_text_.set_color(Utils::create_d2d1_color(0, 0, 0, 255));
}

void Dialog::init(IDWriteFactory* dw_factory)
{
	dw_factory_ = dw_factory;

	right_text_.init(dw_factory_);
	right_text_.set_text(L"Enter to save, esc to cancel", dw_factory_);
	right_text_.set_size(25.0f);
	right_text_.set_position(300.0f, 15.0f);

	left_text_.init(dw_factory_);
}

void Dialog::destroy()
{
	right_text_.destroy();
	left_text_.destroy();
}

void Dialog::set_options(const dialog_options& options)
{
	options_ = options;
}

void Dialog::set_text(const std::wstring& input)
{
	on_going = true;

	input_len_ = input.length();

	left_text_.show();

	left_text_.set_text(input, dw_factory_);
	left_text_.set_size(25.0f);
	left_text_.set_position(10.0f, 15.0f);

	right_text_.show();
}

void Dialog::on_type(const short key)
{
	if (!on_going) return;

	auto text = left_text_.get_text();

	switch (key)
	{
	case 8:
	{
		if (text.length() > input_len_)
		{
			text.pop_back();
			left_text_.set_text(text, dw_factory_);
		}
		break;
	}
	case 13:
	{
		if (on_end && text.length() > input_len_)
		{
			on_going = false;

			right_text_.hide();
			left_text_.hide();

			auto input = text.substr(input_len_, text.length());
			on_end(input);
		}
		break;
	}
	case 27:
		on_going = false;

		right_text_.hide();
		left_text_.hide();

		on_end(L"");
	default:
	{
		if (text.length() - input_len_ < options_.limit)
		{
			if (options_.only_letters && (key > 0x40 && key < 0x5B))
			{
				left_text_.set_text(text.append({ static_cast<wchar_t>(key + 32) }), dw_factory_);
			}
			else if (options_.only_numbers && (key > 0x2F && key < 0x3A))
			{
				left_text_.set_text(text.append({ static_cast<wchar_t>(key) }), dw_factory_);
			}
		}
		break;
	}
	}
}

void Dialog::draw(ID2D1HwndRenderTarget* d2d1_rt, ID2D1SolidColorBrush* d2d1_solidbrush)
{
	left_text_.draw(d2d1_rt, d2d1_solidbrush);
	right_text_.draw(d2d1_rt, d2d1_solidbrush);
}
