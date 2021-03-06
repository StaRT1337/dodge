#include "stdafx.hpp"
#include "include/Utils.hpp"

namespace Utils
{
	D2D1_COLOR_F create_d2d1_color(float r, float g, float b, float a)
	{
		D2D1_COLOR_F color;

		r = static_cast<float>(r);
		g = static_cast<float>(g);
		b = static_cast<float>(b);
		a = static_cast<float>(a);

		color.r = r / 255;
		color.g = g / 255;
		color.b = b / 255;
		color.a = a / 255;

		return color;
	}

	bool cmp_d2d1_color(const D2D1_COLOR_F& color1, const D2D1_COLOR_F& color2)
	{
		return (color1.r == color2.r && color1.g == color2.g && color1.b == color2.b && color1.a == color2.a);
	}

	bool has_coin(const Cube& cube, std::vector<Coin>* coins)
	{
		auto iter = std::find_if(coins->begin(), coins->end(), [&cube = cube](Coin& coin) {
			return coin.get_cube() == cube;
		});

		return iter != coins->end();
	}

	void get_keys(std::vector<bool>* keys)
	{
		for (auto i = 0u; i < 256; ++i)
		{
			keys->at(i) = GetKeyState(i) < 0;
		}
	}

	void map_click(IDWriteFactory* dw_factory, Map* map, Button* button, std::vector<std::pair<Button, std::wstring>>* maps)
	{
		for (auto& pair : *maps)
		{
			if (pair.first == *button)
			{
				std::wstring_convert<std::codecvt_utf8<wchar_t>> conv;
				map->set_map(dw_factory, conv.to_bytes(pair.second));
			}
		}
	}

	void button_hover(Button* sender)
	{
		sender->set_button_color(create_d2d1_color(186, 186, 186, 255));
	}

	const std::string get_str_from_click(const click_type& type)
	{
		switch (type)
		{
		case click_type::ADD_CUBE:
			return "Add border cube";
		case click_type::REMOVE_CUBE:
			return "Remove cube";
		case click_type::SPAWN_CUBE:
			return "Add spawn cube";
		case click_type::END_CUBE:
			return "Add end cube";
		case click_type::ADD_COIN:
			return "Add coin";
		case click_type::REMOVE_COIN:
			return "Remove coin";
		case click_type::ADD_ENEMY:
			return "Add enemy";
		case click_type::REMOVE_ENEMY:
			return "Remove enemy";
		case click_type::ADD_SPINNING_ENEMY:
			return "Add spinning enemy";
		case click_type::REMOVE_SPINNING_ENEMY:
			return "Remove spinning enemy";
		}
	}

	Cube get_cube(const float x, const float y, std::vector<Cube>* cubes)
	{
		for (auto& cube : *cubes)
		{
			auto x1 = x - cube.get_position().x;
			auto y1 = y - cube.get_position().y;

			if ((x1 <= 30 && x1 > 0) && (y1 <= 30 && y1 > 0))
			{
				return cube;
			}
		}
	}
}
