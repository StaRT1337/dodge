#include "stdafx.hpp"

#include "include/Map.hpp"
#include "include/Utils.hpp"

void Map::destroy()
{
	save_button.destroy();
	menu_button.destroy();
}

void Map::on_type(const short key)
{
	if (key == -1) return;

	switch (key)
	{
	case 13:
		if (enemy_.has_points)
		{
			enemy_.finished = true;
			enemy_.save();
			
			enemies_.emplace_back(enemy_);
			enemy_ = Enemy();
		}
		break;
	case 27:
		enemy_.clear();
		enemy_.has_points = false;
		break;
	}

	if (!saving_) return;

	switch (key)
	{
	case 8:
	{
		auto text = left_text_.get_text();
		text.pop_back();

		left_text_.set_text(text, dw_factory_);
		break;
	}
	case 27:
		waiting_for_input_ = false;
		saving_ = false;

		left_text_.hide();

		save_button.show();
		menu_button.show();
		break;
	case 13:
	{
		auto name = left_text_.get_text().substr(6, left_text_.get_text().length());

		std::ofstream file(fmt::format("maps/{}.dodgemap", conv_.to_bytes(name)), std::ios::out | std::ios::trunc | std::ios::binary);

		if (!file)
		{
			std::exception("Couldn't open file");
		}

		if (!savemap_.SerializeToOstream(&file))
		{
			std::exception("Couldn't parse to ofstream");
		}

		left_text_.hide();

		menu_button.show();
		save_button.show();

		waiting_for_input_ = false;
		saving_ = false;
		break;
	}
	default:
	{
		if (waiting_for_input_ && left_text_.get_text().length() - 6 < 9)
		{
			if (key < 0x41 || key > 0x5A) return;

			auto text = left_text_.get_text();
			left_text_.set_text(fmt::format(L"{}{}", text, static_cast<char>(key + 32)), dw_factory_);
		}
		break;
	}
	}
	
}

void Map::on_wheel(const short delta)
{
	if (delta == 0 || delta < -10000) return;

	auto int_type = static_cast<std::uint32_t>(type_);

	if (delta > 0)
	{
		if (int_type + 1 == static_cast<std::uint32_t>(click_type::last))
		{
			type_ = static_cast<click_type>(static_cast<std::uint32_t>(click_type::first) + 1);
		}
		else
		{
			type_ = static_cast<click_type>(int_type + 1);
		}
	}
	else
	{
		if (int_type - 1 == static_cast<std::uint32_t>(click_type::first))
		{
			type_ = static_cast<click_type>(static_cast<std::uint32_t>(click_type::last) - 1);
		}
		else
		{
			type_ = static_cast<click_type>(int_type - 1);
		}
	}

	right_text_.set_text(conv_.from_bytes(fmt::format("Click type: {}", Utils::get_str_from_click(type_))), dw_factory_);
}

void Map::on_click(const POINT& mouse_position, const mouse_type& type)
{
	mouse_position_ = mouse_position;

	if (type == mouse_type::NONE || saving_)  return;

	if (!clicked_)
	{
		clicked_ = true;
		return;
	}

	save_button.check_click(mouse_position_, type);
	menu_button.check_click(mouse_position_, type);

	auto cube = Utils::get_cube(mouse_position_.x, mouse_position_.y, &cubes_);
	if (cube.get_vec_pos() == 0xcccccccc || cube.get_position().y < 60 || cube.get_position().y > 420) return;

	switch (type_)
	{
	case click_type::ADD_CUBE:
		if (Utils::has_coin(cube, &coins_)) break;
		cube.set_type(cube_type::BORDER_CUBE);
		break;
	case click_type::REMOVE_CUBE:
		if (Utils::has_coin(cube, &coins_)) break;
		cube.set_type(cube_type::REGULAR_CUBE);
		break;
	case click_type::SPAWN_CUBE:
		if (Utils::has_coin(cube, &coins_)) break;
		cube.set_type(cube_type::SPAWN_CUBE);
		break;
	case click_type::END_CUBE:
		if (Utils::has_coin(cube, &coins_)) break;
		cube.set_type(cube_type::END_CUBE);
	case click_type::ADD_COIN:
	{
		if (cube.get_type() != cube_type::REGULAR_CUBE) break;

		bool same = false;

		for (auto& coin : coins_)
		{
			if (coin.get_cube() == cube)
			{
				same = true;
			}
		}

		if (same) break;

		Coin coin;
		coin.set_cube(cube);

		coins_.emplace_back(coin);
		break;
	}
	case click_type::REMOVE_COIN:
	{
		if (cube.get_type() != cube_type::REGULAR_CUBE || coins_.size() == 0) break;

		auto iter = std::find_if(coins_.begin(), coins_.end(), [&cube = cube](Coin& coin) {
			return cube == coin.get_cube();
			});

		if (iter != coins_.end())
		{
			coins_.erase(iter);
		}
		break;
	}
	case click_type::ADD_ENEMY:
	{
		if (cube.get_type() != cube_type::REGULAR_CUBE) break;

		auto cube_x = cube.get_position().x;
		auto cube_y = cube.get_position().y;

		auto point = D2D1::Point2F(cube_x + 15, cube_y + 15);

		if (enemy_.has_point(point)) break;

		if (enemy_.has_points)
		{
			auto last_point = enemy_.last_point();

			if (point.x != last_point.x && point.y != last_point.y) break;
		}

		enemy_.add_point(point);
		break;
	}
	case click_type::REMOVE_ENEMY:
	{
		auto iter = std::find_if(enemies_.begin(), enemies_.end(), [&cube = cube, &cubes = cubes_](Enemy& enemy) {
			return Utils::get_cube(enemy.get_position().x, enemy.get_position().y, &cubes) == cube;
		});

		if (iter != enemies_.end())
		{
			enemies_.erase(iter);
		}
		break;
	}
	}

	changed_ = true;
	cube.update(&cubes_);
}

void Map::create_new(IDWriteFactory* dw_factory)
{
	setup(dw_factory);

	cubes_.clear();
	cubes_.shrink_to_fit();

	coins_.clear();
	coins_.shrink_to_fit();

	enemies_.clear();
	enemies_.shrink_to_fit();

	enemy_ = Enemy();

	is_new_ = true;

	Cube cube;
	cube.set_size(30, 30);

	for (auto y = 0u; y < 510; y += 30)
	{
		for (auto x = 0u; x < 750; x += 30)
		{
			cube.set_position(x, y);

			if (y < 60 || y > 420)
			{
				cube.set_type(cube_type::BORDER_CUBE);
			}
			else
			{
				cube.set_type(cube_type::REGULAR_CUBE);
			}

			cube.set_pos(cubes_.size());
			cubes_.push_back(cube);
		}
	} 
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
		throw std::exception("Couldn't parse map.");
	}

	file.close();

	map_name_ = map_name;

	cubes_.clear();
	cubes_.shrink_to_fit();

	coins_.clear();
	coins_.shrink_to_fit();

	enemies_.clear();
	enemies_.shrink_to_fit();

	enemy_ = Enemy();

	is_new_ = false;

	setup(dw_factory);

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

		enemy.save();
		enemy.finished = true;

		enemies_.emplace_back(enemy);
	}
}

void Map::save_map()
{
	if (!changed_) return;

	saving_ = true;

	savemap_.clear_cubes();
	savemap_.clear_coins();

	for (auto& cube : cubes_)
	{
		auto position = new map::point_2f();
		auto p_cube = savemap_.add_cubes();

		position->set_x(cube.get_position().x);
		position->set_y(cube.get_position().y);

		p_cube->set_allocated_position(position);

		switch (cube.get_type())
		{
		case cube_type::BORDER_CUBE:
			p_cube->set_type(map::Cube_cube_type::Cube_cube_type_BORDER_CUBE);
			break;
		case cube_type::REGULAR_CUBE:
			p_cube->set_type(map::Cube_cube_type::Cube_cube_type_REGULAR_CUBE);
			break;
		case cube_type::SPAWN_CUBE:
			p_cube->set_type(map::Cube_cube_type::Cube_cube_type_SPAWN_CUBE);
			break;
		case cube_type::END_CUBE:
			p_cube->set_type(map::Cube_cube_type::Cube_cube_type_END_CUBE);
			break;
		}
	}

	for (auto& coin : coins_)
	{
		auto position = new map::point_2f();
		auto p_coin = savemap_.add_coins();

		position->set_x(coin.get_position().x);
		position->set_y(coin.get_position().y);

		p_coin->set_allocated_position(position);
	}

	for (auto& enemy : enemies_)
	{
		auto p_enemy = savemap_.add_enemies();
		auto points = enemy.get_points();

		for (const auto& point : points)
		{
			auto p_point = p_enemy->add_points();

			p_point->set_x(point.x);
			p_point->set_y(point.y);
		}
	}

	if (is_new_)
	{
		save_button.hide();
		menu_button.hide();

		left_text_.set_text(L"Name: ", dw_factory_);
		left_text_.set_size(25.0f);
		left_text_.show();

		waiting_for_input_ = true;
	}
	else
	{
		std::ofstream file(fmt::format("maps/{}.dodgemap", map_name_), std::ios::out | std::ios::trunc | std::ios::binary);

		if (!file)
		{
			throw std::exception("Map not found.");
		}

		if (!savemap_.SerializeToOstream(&file))
		{
			throw std::exception("Couldn't serialize map to ostream.");
		}

		saving_ = false;
	}

	changed_ = false;
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

	enemy_.draw(d2d1_rt, d2d1_solidbrush, mouse_position_);

	for (auto& enemy : enemies_)
	{
		enemy.draw(d2d1_rt, d2d1_solidbrush, mouse_position_);
	}

	save_button.draw(d2d1_rt, d2d1_solidbrush);
	menu_button.draw(d2d1_rt, d2d1_solidbrush);

	left_text_.draw(d2d1_rt, d2d1_solidbrush);
	right_text_.draw(d2d1_rt, d2d1_solidbrush);
}

void Map::setup(IDWriteFactory* dw_factory)
{
	dw_factory_ = dw_factory;
	type_ = click_type::ADD_CUBE;

	clicked_ = false;
	changed_ = false;

	saving_ = false;
	waiting_for_input_ = false;

	left_text_.init(dw_factory_);

	left_text_.set_text(L"dummy", dw_factory_);
	left_text_.set_size(25.0f);
	left_text_.set_position(25, 15);
	left_text_.set_color(Utils::create_d2d1_color(0, 0, 0, 255));
	left_text_.hide();

	right_text_.init(dw_factory_);

	right_text_.set_text(conv_.from_bytes(fmt::format("Click type: {}", Utils::get_str_from_click(type_))), dw_factory_);
	right_text_.set_position(400, 15);
	right_text_.set_size(25.0f);
	right_text_.set_color(Utils::create_d2d1_color(0, 0, 0, 255));

	save_button.init(dw_factory_);

	save_button.set_position(100, 8);
	save_button.set_text(L"Save", dw_factory_);
	save_button.set_text_size(25.0f);

	save_button.on_hover = Utils::button_hover;
	save_button.on_click = [this](Button* sender, const mouse_type& button) {
		this->save_map();
	};

	menu_button.init(dw_factory_);

	menu_button.set_position(10, 8);
	menu_button.set_text(L"Menu", dw_factory_);
	menu_button.set_text_size(25.0f);

	menu_button.on_hover = Utils::button_hover;
}
