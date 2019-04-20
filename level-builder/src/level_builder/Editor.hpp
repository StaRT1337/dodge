#pragma once

#include "Button.hpp"
#include "Map.hpp"

class Editor
{
private:
	ID2D1Factory* d2d1_factory;
	ID2D1HwndRenderTarget* d2d1_rt;
	ID2D1SolidColorBrush* d2d1_solidbrush;
	IDWriteFactory* dw_factory;

	std::string _window_name;
	std::string _class_name;

	std::vector<bool> held_keys;
	std::vector<std::pair<bool, short>> keys;

	std::vector<std::pair<Button, std::wstring>> maps;

	Map _map;

	HWND _hwnd;
	HRESULT _render_result;
	PAINTSTRUCT _ps;
	POINT mouse_position;

	mouse_type _mouse_type;

	unsigned int _width;
	unsigned int _height;

	short wheel_delta;

	static long __stdcall window_proc_fake(HWND hwnd, std::uint32_t msg, std::uintptr_t w_param, long l_param);
	long __stdcall window_proc(HWND hwnd, std::uint32_t msg, std::uintptr_t w_param, long l_param);

	void create_resources();
	void delete_resources();

	void begin_draw();
	void end_draw();

	void update();
	void render();
public:
	Editor();
	~Editor();

	Button maps_button;
	Button new_map_button;

	void init(HINSTANCE instance, HINSTANCE prev_instance, char* cmd_line, int cmd_show);
	void run();

	void init_maps();

	bool in_menu;
	bool is_running;
};
