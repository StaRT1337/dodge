#include "stdafx.hpp"
#include "include/Utils.hpp"

#include "include/Map.hpp"

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
