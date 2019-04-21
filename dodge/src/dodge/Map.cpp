#include "stdafx.hpp"

#include "include/Map.hpp"
#include "include/Utils.hpp"

void Map::destroy()
{
	menu_button.destroy();
}

void Map::on_type(std::vector<bool>* keys)
{
	player_.on_type(keys, &cubes_, &coins_);
}

void Map::set_map(IDWriteFactory* dw_factory, const std::string& map_name)
{
	std::ifstream file(fmt::format("maps/{}.dodgemap", map_name), std::ios::in | std::ios::binary);

	if (!file)
	{
		throw std::exception("Map not found.");
	}

	map::map _map;
	if (!_map.ParseFromIstream(&file))
	{
		throw std::exception("Couldn't parse map");
	}

	file.close();

	map_name_ = map_name;

	cubes_.clear();
	cubes_.shrink_to_fit();

	coins_.clear();
	coins_.shrink_to_fit();

	dw_factory_ = dw_factory;

	menu_button.init(dw_factory_);

	menu_button.set_position(10, 8);
	menu_button.set_text(L"Menu", dw_factory_);
	menu_button.set_text_size(25.0f);
	menu_button.on_hover = Utils::button_hover;

	Cube cube;
	cube.set_size(30, 30);

	for (const auto& p_cube : _map.cubes())
	{
		cube.set_position(p_cube.x(), p_cube.y());

		switch (p_cube.type())
		{
		case map::Cube_cube_type::Cube_cube_type_BORDER_CUBE:
			cube.set_type(cube_type::BORDER_CUBE);
			break;
		case map::Cube_cube_type::Cube_cube_type_REGULAR_CUBE:
			cube.set_type(cube_type::REGULAR_CUBE);
			break;
		case map::Cube_cube_type::Cube_cube_type_SPAWN_CUBE:
			cube.set_type(cube_type::SPAWN_CUBE);
			break;
		case map::Cube_cube_type::Cube_cube_type_END_CUBE:
			cube.set_type(cube_type::END_CUBE);
			break;
		}

		cubes_.emplace_back(cube);
	}

	Coin coin;

	for (const auto& p_coin : _map.coins())
	{
		auto cube = Utils::get_cube(p_coin.x(), p_coin.y(), &cubes_);
		coin.set_cube(cube);

		coins_.emplace_back(coin, false);
	}

	player_.start(&cubes_, &coins_);
}

void Map::draw(ID2D1HwndRenderTarget* d2d1_rt, ID2D1SolidColorBrush* d2d1_solidbrush)
{
	if (cubes_.size() >= 425)
	{
		for (auto& cube : cubes_)
		{
			cube.draw(d2d1_rt, d2d1_solidbrush);
		}
	}

	for (auto& pair : coins_)
	{
		if (!pair.second)
		{
			pair.first.draw(d2d1_rt, d2d1_solidbrush);
		}
	}

	menu_button.draw(d2d1_rt, d2d1_solidbrush);
	player_.draw(d2d1_rt, d2d1_solidbrush);
}
