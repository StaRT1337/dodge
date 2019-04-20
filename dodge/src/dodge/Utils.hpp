#pragma once

#include "Button.hpp"
#include "Map.hpp"
#include "Cube.hpp"

namespace Utils
{
	D2D1_COLOR_F create_d2d1_color(float r, float g, float b, float a);

	bool cmp_d2d1_color(const D2D1_COLOR_F& color1, const D2D1_COLOR_F& color2);

	void get_keys(std::vector<bool>* keys);

	void map_click(IDWriteFactory* dw_factory, Map* map, Button* button, std::vector<std::pair<Button, std::wstring>>* maps);
	void button_hover(Button* sender);

	//const std::string get_str_from_click(const click_type& type);

	Cube get_cube(const POINT& mouse_position, std::vector<Cube>* cubes);

	template <typename T>
	inline void safe_release(T** ptr)
	{
		if (*ptr)
		{
			(*ptr)->Release();
			*ptr = 0;
		}
	}
}
