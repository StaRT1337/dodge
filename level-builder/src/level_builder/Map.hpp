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
	IDWriteFactory* dw_factory_;
	std::wstring_convert<std::codecvt_utf8<wchar_t>> conv_;

<<<<<<< HEAD
	std::string map_name_;
=======
	std::string _map_name;

	std::vector<Cube> _cubes;
	std::vector<Coin> coins_;
>>>>>>> master

	std::vector<Cube> cubes_;
	std::vector<Coin> coins_;

	click_type type_;

	Text right_text_;
	Text left_text_;

	map::map savemap_;

	bool is_new_;
	bool clicked_;
	bool changed_;

	bool saving_;
	bool waiting_for_input_;

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
