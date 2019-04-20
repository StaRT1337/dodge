#include "stdafx.hpp"
#include "Map.hpp"

#include "Utils.hpp"

void Map::destroy()
{
	save_button.destroy();
	menu_button.destroy();
}

void Map::on_type(std::vector<std::pair<bool, short>>* keys)
{
	if (!_saving) return;

	std::vector<std::pair<bool, short>> pressed;

	std::copy_if(keys->begin(), keys->end(), std::back_inserter(pressed), [](std::pair<bool, short> pair) {
		return pair.first;
	});

	if (pressed.size() > 0)
	{
		auto front = pressed.front();

		switch (front.second)
		{
		case 8:
		{
			auto text = _left_text.get_text();
			text.pop_back();

			_left_text.set_text(text, _dw_factory);
			break;
		}
		case 27:
			_waiting_for_input = false;
			_saving = false;

			_left_text.hide();

			save_button.show();
			menu_button.show();
			break;
		case 13:
		{
			auto name = _left_text.get_text().substr(6, _left_text.get_text().length());

			std::ofstream file(fmt::format("maps/{}.dodgemap", _conv.to_bytes(name)), std::ios::out | std::ios::trunc | std::ios::binary);

			if (!file)
			{
				std::exception("Couldn't open file");
			}

			if (!_savemap.SerializeToOstream(&file))
			{
				std::exception("Couldn't parse to ofstream");
			}

			_left_text.hide();

			menu_button.show();
			save_button.show();

			_waiting_for_input = false;
			_saving = false;
			break;
		}
		default:
		{
			if (_waiting_for_input && _left_text.get_text().length() - 6 < 9)
			{
				if (front.second < 0x41 || front.second > 0x5A) return;

				auto text = _left_text.get_text();
				_left_text.set_text(fmt::format(L"{}{}", text, static_cast<char>(pressed.front().second + 32)), _dw_factory);
			}
			break;
		}
		}
	}
}

void Map::on_wheel(const short delta)
{
	if (delta == 0 || delta < -10000) return;

	auto int_type = static_cast<std::uint32_t>(_type);

	if (delta > 0)
	{
		if (int_type + 1 == static_cast<std::uint32_t>(click_type::last))
		{
			_type = static_cast<click_type>(static_cast<std::uint32_t>(click_type::first) + 1);
		}
		else
		{
			_type = static_cast<click_type>(int_type + 1);
		}
	}
	else
	{
		if (int_type - 1 == static_cast<std::uint32_t>(click_type::first))
		{
			_type = static_cast<click_type>(static_cast<std::uint32_t>(click_type::last) - 1);
		}
		else
		{
			_type = static_cast<click_type>(int_type - 1);
		}
	}

	_right_text.set_text(_conv.from_bytes(fmt::format("Click type: {}", Utils::get_str_from_click(_type))), _dw_factory);
}

void Map::on_click(const POINT& mouse_position, const mouse_type& type)
{
	if (type == mouse_type::NONE || _saving)  return;

	if (!_clicked)
	{
		_clicked = true;
		return;
	}

	save_button.check_click(mouse_position, type);
	menu_button.check_click(mouse_position, type);

	auto cube = Utils::get_cube(mouse_position, &_cubes);
	if (cube.get_vec_pos() == 0xcccccccc || cube.get_position().y < 60 || cube.get_position().y > 420) return;

	switch (_type)
	{
	case click_type::ADD_CUBE:
		cube.set_type(cube_type::BORDER_CUBE);
		break;
	case click_type::REMOVE_CUBE:
		cube.set_type(cube_type::REGULAR_CUBE);
		break;
	case click_type::SPAWN_CUBE:
		cube.set_type(cube_type::SPAWN_CUBE);
		break;
	}

	_changed = true;
	cube.update(&_cubes);
}

void Map::create_new(IDWriteFactory* dw_factory)
{
	setup(dw_factory);

	_cubes.clear();

	_is_new = true;

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

			cube.set_pos(_cubes.size());
			_cubes.push_back(cube);
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

	_map_name = map_name;

	_cubes.clear();
	_is_new = false;

	setup(dw_factory);

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
		}

		cube.set_pos(_cubes.size());
		_cubes.push_back(cube);
	}
}

void Map::save_map()
{
	if (!_changed) return;

	_saving = true;
	_savemap.clear_cubes();

	for (auto& cube : _cubes)
	{
		auto p_cube = _savemap.add_cubes();

		p_cube->set_x(cube.get_position().x);
		p_cube->set_y(cube.get_position().y);

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
		}
	}

	if (_is_new)
	{
		save_button.hide();
		menu_button.hide();

		_left_text.set_text(L"Name: ", _dw_factory);
		_left_text.set_size(25.0f);
		_left_text.show();

		_waiting_for_input = true;
	}
	else
	{
		std::ofstream file(fmt::format("maps/{}.dodgemap", _map_name), std::ios::out | std::ios::trunc | std::ios::binary);

		if (!file)
		{
			throw std::exception("Map not found.");
		}

		if (!_savemap.SerializeToOstream(&file))
		{
			throw std::exception("Couldn't serialize map to ostream.");
		}

		_saving = false;
	}

	_changed = false;
}

void Map::draw(ID2D1HwndRenderTarget* d2d1_rt, ID2D1SolidColorBrush* d2d1_solidbrush)
{
	if (_cubes.size() >= 425)
	{
		for (auto& cube : _cubes)
		{
			cube.draw(d2d1_rt, d2d1_solidbrush);
		}
	}

	save_button.draw(d2d1_rt, d2d1_solidbrush);
	menu_button.draw(d2d1_rt, d2d1_solidbrush);

	_left_text.draw(d2d1_rt, d2d1_solidbrush);
	_right_text.draw(d2d1_rt, d2d1_solidbrush);
}

void Map::setup(IDWriteFactory* dw_factory)
{
	_dw_factory = dw_factory;
	_type = click_type::ADD_CUBE;

	_clicked = false;
	_changed = false;

	_saving = false;
	_waiting_for_input = false;

	_left_text.init(dw_factory);

	_left_text.set_text(L"dummy", dw_factory);
	_left_text.set_size(25.0f);
	_left_text.set_position(25, 15);
	_left_text.set_color(Utils::create_d2d1_color(0, 0, 0, 255));
	_left_text.hide();

	_right_text.init(dw_factory);

	_right_text.set_text(_conv.from_bytes(fmt::format("Click type: {}", Utils::get_str_from_click(_type))), dw_factory);
	_right_text.set_position(400, 15);
	_right_text.set_size(25.0f);
	_right_text.set_color(Utils::create_d2d1_color(0, 0, 0, 255));

	save_button.init(dw_factory);

	save_button.set_position(100, 8);
	save_button.set_text(L"Save", dw_factory);
	save_button.set_text_size(25.0f);

	save_button.on_hover = Utils::button_hover;
	save_button.on_click = [this](Button* sender, const mouse_type& button) {
		this->save_map();
	};

	menu_button.init(dw_factory);

	menu_button.set_position(10, 8);
	menu_button.set_text(L"Menu", dw_factory);
	menu_button.set_text_size(25.0f);

	menu_button.on_hover = Utils::button_hover;
}
