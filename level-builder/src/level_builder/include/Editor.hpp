#pragma once

#include "Button.hpp"
#include "Map.hpp"

class Editor
{
private:
	ID2D1Factory* d2d1_factory_;
	ID2D1HwndRenderTarget* d2d1_rt_;
	ID2D1SolidColorBrush* d2d1_solidbrush_;
	IDWriteFactory* dw_factory_;

	std::string window_name_;
	std::string class_name_;

	std::vector<std::pair<Button, std::wstring>> maps_;

	Map map_;

	HWND hwnd_;
	HRESULT render_result_;
	PAINTSTRUCT ps_;
	POINT mouse_position_;

	mouse_type mouse_type_;

	unsigned int width_;
	unsigned int height_;

	short wheel_delta_;
	short key_;

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
