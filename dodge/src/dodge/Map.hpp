#pragma once
#include "Cube.hpp"
#include "Button.hpp"
#include "Player.hpp"

#include "map.pb.h"

enum class click_type : std::uint32_t
{
	first,
	ADD_CUBE,
	REMOVE_CUBE,
	SPAWN_CUBE,
	last
};

class Map
{
private:
	Player player_;

	IDWriteFactory* dw_factory_;
	std::wstring_convert<std::codecvt_utf8<wchar_t>> conv_;

	std::string map_name_;
	std::vector<Cube> cubes_;

	click_type type_;

	map::map savemap_;

	//void setup(IDWriteFactory* dw_factory);
public:
	Button menu_button;

	void destroy();

	void on_type(std::vector<bool>* keys);

	void set_map(IDWriteFactory* dw_factory, const std::string& map_name);
	void draw(ID2D1HwndRenderTarget* d2d1_rt, ID2D1SolidColorBrush* d2d1_solidbrush);
};
