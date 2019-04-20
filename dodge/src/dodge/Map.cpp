#include "stdafx.hpp"
#include "Map.hpp"

#include "Utils.hpp"

void Map::destroy()
{
	menu_button.destroy();
}

void Map::on_type(std::vector<bool>* keys)
{
	player_.on_type(keys, &cubes_);
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
	dw_factory_ = dw_factory;

	menu_button.init(dw_factory_);

	menu_button.set_position(10, 8);
	menu_button.set_text(L"Menu", dw_factory_);
	menu_button.set_text_size(25.0f);
	menu_button.on_hover = Utils::button_hover;

	std::vector<std::pair<int, int>> spawn_cubes;

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
			spawn_cubes.emplace_back(p_cube.x(), p_cube.y());

			break;
		}

		cube.set_pos(cubes_.size());
		cubes_.push_back(cube);
	}

	auto x = (spawn_cubes.back().first + spawn_cubes.front().first) / 2;
	auto y = (spawn_cubes.back().second + spawn_cubes.front().second) / 2;

	if (Utils::get_cube(POINT{ x, y }, &cubes_).get_type() != cube_type::SPAWN_CUBE)
	{
		std::vector<std::pair<int, int>> out;
		std::sample(spawn_cubes.begin(), spawn_cubes.end(), std::back_inserter(out), 1, std::mt19937{ std::random_device{}() });

		x = out.front().first;
		y = out.front().second;
	}

	player_.set_position(x, y);
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

	menu_button.draw(d2d1_rt, d2d1_solidbrush);
	player_.draw(d2d1_rt, d2d1_solidbrush);
}
