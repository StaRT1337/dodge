#pragma once
#include "Cube.hpp"
#include "Button.hpp"
#include "Text.hpp"
#include "Coin.hpp"

#include "map.pb.h"

enum class click_type : std::uint32_t
{
	first,
	ADD_CUBE,
	REMOVE_CUBE,
	SPAWN_CUBE,
	END_CUBE,
	ADD_COIN,
	REMOVE_COIN,
	last
};

class Map
{
private:
	IDWriteFactory* _dw_factory;
	std::wstring_convert<std::codecvt_utf8<wchar_t>> _conv;

	std::string _map_name;

	std::vector<Cube> _cubes;
	std::vector<Coin> coins_;

	click_type _type;

	Text _right_text;
	Text _left_text;

	map::map _savemap;

	bool _is_new;
	bool _clicked;
	bool _changed;

	bool _saving;
	bool _waiting_for_input;

	void setup(IDWriteFactory* dw_factory);
public:
	Button save_button;
	Button menu_button;

	void destroy();

	void on_type(std::vector<std::pair<bool, short>>* keys);
	void on_wheel(const short delta);
	void on_click(const POINT& mouse_position, const mouse_type& type);

	void create_new(IDWriteFactory* dw_factory);
	void set_map(IDWriteFactory* dw_factory, const std::string& map_name);

	void save_map();
	void draw(ID2D1HwndRenderTarget* d2d1_rt, ID2D1SolidColorBrush* d2d1_solidbrush);
};
