#include "stdafx.hpp"

#include "include/Map.hpp"
#include "include/Utils.hpp"

void Map::destroy()
{
	save_button.destroy();
	menu_button.destroy();

	dialog_.destroy();
}

void Map::on_type(const short key)
{
	if (key == -1) return;

	dialog_.on_type(key);

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

	if (type == mouse_type::NONE || saving_ || dialog_.on_going)  return;

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
		spawn_cube(&cube, cube_type::BORDER_CUBE);
		break;
	case click_type::REMOVE_CUBE:
		spawn_cube(&cube, cube_type::REGULAR_CUBE);
		break;
	case click_type::SPAWN_CUBE:
		spawn_cube(&cube, cube_type::SPAWN_CUBE);
		break;
	case click_type::END_CUBE:
		spawn_cube(&cube, cube_type::END_CUBE);
		break;
	case click_type::ADD_COIN:
	{
		if (cube.get_type() != cube_type::REGULAR_CUBE) break;

		auto spin_iter = std::find_if(spinning_enemies_.begin(), spinning_enemies_.end(), [&cube = cube](SpinningEnemy& enemy) {
			return cube.get_position().x + 15 == enemy.get_position().x && cube.get_position().y + 15 == enemy.get_position().y;
		});

		if (spin_iter != spinning_enemies_.end()) break; 

		auto coin_iter = std::find_if(coins_.begin(), coins_.end(), [&cube = cube](Coin& coin) {
			return coin.get_cube() == cube;
		});

		if (coin_iter != coins_.end()) break;

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
			coins_.shrink_to_fit();
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
		if (cube.get_type() != cube_type::REGULAR_CUBE) break;

		auto iter = std::find_if(enemies_.begin(), enemies_.end(), [&cube = cube, &cubes = cubes_](Enemy& enemy) {
			return Utils::get_cube(enemy.get_position().x, enemy.get_position().y, &cubes) == cube;
		});

		if (iter != enemies_.end())
		{
			enemies_.erase(iter);
			enemies_.shrink_to_fit();
		}
		break;
	}
	case click_type::ADD_SPINNING_ENEMY:
	{
		if (cube.get_type() != cube_type::REGULAR_CUBE) break;

		auto iter = std::find_if(spinning_enemies_.begin(), spinning_enemies_.end(), [&cube = cube](SpinningEnemy& enemy) {
			return cube.get_position().x == enemy.get_position().x - 15 && cube.get_position().y == enemy.get_position().y - 15;
		});

		if (iter == spinning_enemies_.end())
		{
			spinning_enemy_.set_position(cube.get_position().x + 15, cube.get_position().y + 15);

			menu_button.hide();
			save_button.hide();

			right_text_.hide();

			dialog_options options;
			options.limit = 2;
			options.only_letters = false;
			options.only_numbers = true;

			dialog_.set_options(options);
			dialog_.set_text(L"Amount: ");

			dialog_type_ = dialog_type::SPINNING_ENEMY;
		}
		break;
	}
	case click_type::REMOVE_SPINNING_ENEMY:
	{
		if (cube.get_type() != cube_type::REGULAR_CUBE) break;

		auto iter = std::find_if(spinning_enemies_.begin(), spinning_enemies_.end(), [&cube = cube](SpinningEnemy& enemy) {
			return cube.get_position().x == enemy.get_position().x - 15 && cube.get_position().y == enemy.get_position().y - 15;
		});

		if (iter != spinning_enemies_.end())
		{
			spinning_enemies_.erase(iter);
			spinning_enemies_.shrink_to_fit();
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

	SpinningEnemy enemy;

	for (const auto& p_enemy : _map.spinning_enemies())
	{
		auto position = p_enemy.position();
		enemy.set_position(position.x(), position.y());

		enemy.amount = p_enemy.amount();
		enemy.spacing = p_enemy.spacing();
		enemy.speed = p_enemy.speed();

		enemy.set_enemies();
		spinning_enemies_.emplace_back(enemy);
	}
}

void Map::save_map()
{
	if (!changed_) return;

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

	for (auto& enemy : spinning_enemies_)
	{
		auto position = new map::point_2f();
		auto p_enemy = savemap_.add_spinning_enemies();

		position->set_x(enemy.get_position().x);
		position->set_y(enemy.get_position().y);

		p_enemy->set_allocated_position(position);
		p_enemy->set_amount(enemy.amount);
		p_enemy->set_spacing(enemy.spacing);
		p_enemy->set_speed(enemy.speed);
	}

	if (is_new_)
	{
		/*save_button.hide();
		menu_button.hide();

		left_text_.set_text(L"Name: ", dw_factory_);
		left_text_.set_size(25.0f);
		left_text_.show();

		waiting_for_input_ = true; */

		menu_button.hide();
		save_button.hide();

		right_text_.hide();

		dialog_type_ = dialog_type::MAP_SAVE;

		dialog_options options;
		options.limit = 8;
		options.only_letters = true;
		options.only_numbers = false;

		dialog_.set_options(options);
		dialog_.set_text(L"Name: ");
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

	for (auto& spinning_enemy : spinning_enemies_)
	{
		spinning_enemy.draw(d2d1_rt, d2d1_solidbrush);
	}

	save_button.draw(d2d1_rt, d2d1_solidbrush);
	menu_button.draw(d2d1_rt, d2d1_solidbrush);

	left_text_.draw(d2d1_rt, d2d1_solidbrush);
	right_text_.draw(d2d1_rt, d2d1_solidbrush);

	dialog_.draw(d2d1_rt, d2d1_solidbrush);
}

void Map::setup(IDWriteFactory* dw_factory)
{
	dw_factory_ = dw_factory;
	type_ = click_type::ADD_CUBE;

	clicked_ = false;
	changed_ = false;

	saving_ = false;
	waiting_for_input_ = false;

	cubes_ = {};
	coins_ = {};
	enemies_ = {};
	spinning_enemies_ = {};

	enemy_ = Enemy();
	spinning_enemy_ = SpinningEnemy();

	left_text_.init(dw_factory_);

	left_text_.set_text(L"dummy", dw_factory_);
	left_text_.set_size(25.0f);
	left_text_.set_position(25, 15);
	left_text_.set_color(Utils::create_d2d1_color(0, 0, 0, 255));
	left_text_.hide();

	right_text_.init(dw_factory_);

	right_text_.set_text(conv_.from_bytes(fmt::format("Click type: {}", Utils::get_str_from_click(type_))), dw_factory_);
	right_text_.set_position(330, 15);
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

	dialog_.init(dw_factory_);
	dialog_.on_end = [this](const std::wstring& input) {
		menu_button.show();
		save_button.show();

		right_text_.show();

		if (input.empty()) return;

		switch (dialog_type_)
		{
		case dialog_type::MAP_SAVE:
		{
			std::ofstream file(fmt::format("maps/{}.dodgemap", conv_.to_bytes(input)), std::ios::out | std::ios::trunc | std::ios::binary);

			if (!file)
			{
				throw std::exception("Map not found.");
			}

			if (!savemap_.SerializeToOstream(&file))
			{
				throw std::exception("Couldn't serialize map to ostream.");
			}
			break;
		}
		case dialog_type::SPINNING_ENEMY:
		{
			if (spinning_enemy_.amount == -1)
			{
				spinning_enemy_.amount = boost::lexical_cast<unsigned int>(input);

				menu_button.hide();
				save_button.hide();

				right_text_.hide();

				dialog_options options;
				options.limit = 2;
				options.only_letters = false;
				options.only_numbers = true;

				dialog_.set_options(options);
				dialog_.set_text(L"Spacing: ");
			}
			else
			{
				if (spinning_enemy_.spacing == -1.0f)
				{
					spinning_enemy_.spacing = boost::lexical_cast<float>(input);
					spinning_enemy_.set_enemies();

					menu_button.hide();
					save_button.hide();

					right_text_.hide();

					dialog_options options;
					options.limit = 1;
					options.only_letters = false;
					options.only_numbers = true;

					dialog_.set_options(options);
					dialog_.set_text(L"Speed: ");
				}
				else
				{
					spinning_enemy_.speed = boost::lexical_cast<float>(input) / 100.0f;

					spinning_enemies_.emplace_back(spinning_enemy_);
					spinning_enemy_ = SpinningEnemy();
				}
			}
			break;
		}
		}
	};

	menu_button.init(dw_factory_);

	menu_button.set_position(10, 8);
	menu_button.set_text(L"Menu", dw_factory_);
	menu_button.set_text_size(25.0f);

	menu_button.on_hover = Utils::button_hover;
}

void Map::spawn_cube(Cube* cube, const cube_type& type)
{
	if (Utils::has_coin(*cube, &coins_)) return;

	if (type != cube_type::REGULAR_CUBE)
	{
		for (auto& enemy : enemies_)
		{
			if (enemy.cube_in_point(cube))
			{
				return;
			}
		}

		for (auto& enemy : spinning_enemies_)
		{
			if (enemy.get_position().x == cube->get_position().x + 15 && enemy.get_position().y == cube->get_position().y + 15)
			{
				return;
			}
		}
	}

	cube->set_type(type);
	cube->update(&cubes_);
}

