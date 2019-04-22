#include "stdafx.hpp"

#include "include/Map.hpp"
#include "include/Utils.hpp"

void Map::destroy()
{
	menu_button.destroy();
}

void Map::on_type(std::vector<bool>* keys)
{
	player_.on_type(keys, &cubes_, &coins_, &enemies_);
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

	enemies_.clear();
	enemies_.shrink_to_fit();

	dw_factory_ = dw_factory;

	menu_button.init(dw_factory_);

	menu_button.set_position(10, 8);
	menu_button.set_text(L"Menu", dw_factory_);
	menu_button.set_text_size(25.0f);
	menu_button.on_hover = Utils::button_hover;

	Cube cube;
	cube.set_size(30, 30);

	map::point_2f position;

	for (const auto& p_cube : _map.cubes())
	{
		position = p_cube.position();
		cube.set_position(position.x(), position.y());

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

		cube.set_pos(cubes_.size());
		cubes_.emplace_back(cube);
	}

	Coin coin;

	for (const auto& p_coin : _map.coins())
	{
		position = p_coin.position();
		auto cube = Utils::get_cube(position.x(), position.y(), &cubes_);

		coin.set_cube(cube);
		coins_.emplace_back(coin);
	}

	for (const auto& p_enemy : _map.enemies())
	{
		Enemy enemy;

		for (const auto& p_point : p_enemy.points())
		{
			enemy.add_point(D2D1::Point2F(p_point.x(), p_point.y()));
		}

		enemies_.emplace_back(enemy);
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

	for (auto& coin : coins_)
	{
		coin.draw(d2d1_rt, d2d1_solidbrush);
	}

	for (auto& enemy : enemies_)
	{

		enemy.draw(d2d1_rt, d2d1_solidbrush);
	}

	menu_button.draw(d2d1_rt, d2d1_solidbrush);
	player_.draw(d2d1_rt, d2d1_solidbrush);
}
