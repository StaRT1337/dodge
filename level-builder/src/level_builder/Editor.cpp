#include "stdafx.hpp"

#include "include/Editor.hpp"

#include "include/Utils.hpp"
#include "include/Timer.hpp"

Editor* editor;

Editor::Editor()
{
	d2d1_factory_ = 0;
	d2d1_rt_ = 0;
	d2d1_solidbrush_ = 0;

	window_name_ = "Dodge level editor";
	class_name_ = "main_editor_window";

	width_ = 750;
	height_ = 510;

	keys_.resize(256);

#ifdef DEBUG
	AllocConsole();
	AttachConsole(GetCurrentProcessId());

	freopen_s(reinterpret_cast<FILE**>(stdout), "CONOUT$", "w", stdout);
	freopen_s(reinterpret_cast<FILE**>(stdin), "CONIN$", "r", stdin);

	PRINT("Allocated console");
#endif
}

Editor::~Editor()
{
#ifdef DEBUG
	FreeConsole();

	fclose(stdout);
	fclose(stdin);
#endif
	maps_button.destroy();
	new_map_button.destroy();

	for (auto& pair : maps_)
	{
		pair.first.destroy();
	}

	map_.destroy();

	Utils::safe_release(&d2d1_factory_);
	Utils::safe_release(&d2d1_rt_);
	Utils::safe_release(&d2d1_solidbrush_);

	Utils::safe_release(&dw_factory_);

	PostQuitMessage(0);
}

void Editor::init(HINSTANCE instance, HINSTANCE prev_instance, char* cmd_line, int cmd_show)
{
	editor = this;

	WNDCLASSEX wc;
	MSG Msg;
	RECT rc;

	HRESULT hr;

	wc.cbSize = sizeof(WNDCLASSEX);
	wc.style = 0;
	wc.lpfnWndProc = window_proc_fake;
	wc.cbClsExtra = 0;
	wc.cbWndExtra = 0;
	wc.hInstance = instance;
	wc.hIcon = LoadIconA(0, IDI_APPLICATION);
	wc.hCursor = LoadCursorA(0, IDC_ARROW);
	wc.hbrBackground = reinterpret_cast<HBRUSH>(COLOR_WINDOW + 1);
	wc.lpszMenuName = 0;
	wc.lpszClassName = class_name_.data();
	wc.hIconSm = LoadIconA(0, IDI_APPLICATION);

	if (!RegisterClassExA(&wc))
	{
		throw std::exception("Failed to register window class.");
	}

	PRINT("Successfully registered window class");

	RECT window_dimensions;
	window_dimensions.left = 0;
	window_dimensions.top = 0;
	window_dimensions.right = width_;
	window_dimensions.bottom = height_;

	AdjustWindowRect(&window_dimensions, WS_OVERLAPPED | WS_CAPTION | WS_SYSMENU | WS_MINIMIZEBOX, false);

	hwnd_ = CreateWindowExA(
		WS_EX_CLIENTEDGE,
		class_name_.data(),
		window_name_.data(),
		WS_OVERLAPPED | WS_CAPTION | WS_SYSMENU | WS_MINIMIZEBOX,
		CW_USEDEFAULT, CW_USEDEFAULT, window_dimensions.right - window_dimensions.left + 4, window_dimensions.bottom - window_dimensions.top + 4,
		0, 0, instance, 0
	);

	if (hwnd_ == 0)
	{
		throw std::exception("Failed to create window.");
	}
	PRINT("Successfully created window");

	GetWindowRect(hwnd_, &rc);
	SetWindowPos(hwnd_, 0, (GetSystemMetrics(SM_CXSCREEN) - rc.right) / 2, (GetSystemMetrics(SM_CYSCREEN) - rc.bottom) / 2, 0, 0, SWP_NOZORDER | SWP_NOSIZE);

	hr = D2D1CreateFactory(D2D1_FACTORY_TYPE_SINGLE_THREADED, &d2d1_factory_);
	if (FAILED(hr))
	{
		throw std::exception("Failed to create Direct2D factory");
	}

	hr = DWriteCreateFactory(DWRITE_FACTORY_TYPE_SHARED, __uuidof(IDWriteFactory), reinterpret_cast<IUnknown**>(&dw_factory_));
	if (FAILED(hr))
	{
		throw std::exception("Failed to create DirectWrite factory");
	}
	PRINT("Successfully created factories");

	GetClientRect(hwnd_, &rc);
	hr = d2d1_factory_->CreateHwndRenderTarget(
		D2D1::RenderTargetProperties(),
		D2D1::HwndRenderTargetProperties(
			hwnd_,
			D2D1::SizeU(
				rc.right - rc.left,
				rc.bottom - rc.top)
		),
		&d2d1_rt_
	);

	d2d1_rt_->SetTextAntialiasMode(D2D1_TEXT_ANTIALIAS_MODE_GRAYSCALE);

	if (FAILED(hr))
	{
		throw std::exception("Failed to create Direct2D render target.");
	}
	PRINT("Successfully created render target");

	hr = d2d1_rt_->CreateSolidColorBrush(
		D2D1::ColorF(D2D1::ColorF::Black),
		&d2d1_solidbrush_
	);

	if (FAILED(hr))
	{
		throw std::exception("Failed to create Direct2D solid brush.");
	}
	PRINT("Successfully created solid brush");

	maps_button.init(dw_factory_);
	new_map_button.init(dw_factory_);
	
	maps_button.set_text(L"Maps", dw_factory_);
	maps_button.set_position(width_ / 2 - 58, height_ / 2 + 30);
	maps_button.set_text_size(40.0f);
	maps_button.on_hover = Utils::button_hover;

	maps_button.on_click = [this](Button* sender, const mouse_type& button) {
		this->init_maps();
	};


	new_map_button.set_text(L"New map", dw_factory_);
	new_map_button.set_position(width_ / 2 - 94, height_ / 2 - 75);
	new_map_button.set_text_size(40.0f);
	new_map_button.on_hover = Utils::button_hover;

	new_map_button.on_click = [this, &map_ = map_, &dw_factory_ = dw_factory_, &in_menu = in_menu](Button* sender, const mouse_type& button) {
		map_.create_new(dw_factory_);
		in_menu = false;

		map_.menu_button.on_click = [this, &in_menu = in_menu](Button* sender, const mouse_type& button) {
			this->maps_button.show();
			this->new_map_button.show();

			in_menu = true;
		};
	};

	ShowWindow(hwnd_, cmd_show);
	UpdateWindow(hwnd_);

	in_menu = true;
	is_running = true;
}

void Editor::run()
{
	Timer timer;
	MSG msg;

	for (;;)
	{
		if (!is_running) break;

		while (PeekMessageA(&msg, 0, 0, 0, PM_REMOVE))
		{
			TranslateMessage(&msg);
			DispatchMessageA(&msg);
		}

		if (!IsIconic(hwnd_))
		{
			if (in_menu && GetFocus() != hwnd_)
			{
				Sleep((1000.0f / 60.0f) - timer.get_delta());
				continue;
			}

			create_resources();
			begin_draw();

			update();
			render();

			end_draw();
			delete_resources();
		}

		Sleep((1000.0f / 60.0f) - timer.get_delta());
	}
}

LRESULT Editor::window_proc_fake(HWND hwnd, std::uint32_t msg, std::uintptr_t w_param, long l_param)
{
	auto result = editor->window_proc(hwnd, msg, w_param, l_param);
	return result;
}

LRESULT Editor::window_proc(HWND hwnd, std::uint32_t msg, std::uintptr_t w_param, long l_param)
{
	switch (msg)
	{
	case WM_CLOSE:
		is_running = false;
		DestroyWindow(hwnd);
		break;
	case WM_KEYDOWN:
		keys_.at(w_param) = std::pair<bool, int>(true, w_param);
		break;
	case WM_LBUTTONDOWN:
		mouse_type_ = mouse_type::LBUTTON;

		GetCursorPos(&mouse_position_);
		ScreenToClient(hwnd, &mouse_position_);
		break;
	case WM_LBUTTONUP:
		mouse_type_ = mouse_type::NONE;
	case WM_MOUSEMOVE:
		GetCursorPos(&mouse_position_);
		ScreenToClient(hwnd, &mouse_position_);
		break;
	case WM_MOUSEWHEEL:
		wheel_delta_ = GET_WHEEL_DELTA_WPARAM(w_param);
		break;
	case WM_DESTROY:
		PostQuitMessage(0);
		break;
	default:
		return DefWindowProcA(hwnd, msg, w_param, l_param);
	}

	return 0;
}

void Editor::create_resources()
{
	if (d2d1_rt_ == 0)
	{
		RECT rc;

		GetClientRect(hwnd_, &rc);
		d2d1_factory_->CreateHwndRenderTarget(
			D2D1::RenderTargetProperties(),
			D2D1::HwndRenderTargetProperties(
				hwnd_,
				D2D1::SizeU(
					rc.right - rc.left,
					rc.bottom - rc.top)
			),
			&d2d1_rt_
		);
	}

	if (d2d1_solidbrush_ == 0)
	{
		d2d1_rt_->CreateSolidColorBrush(
			D2D1::ColorF(D2D1::ColorF::White),
			&d2d1_solidbrush_
		);
	}
}

void Editor::delete_resources()
{
	if (FAILED(render_result_) || render_result_ == D2DERR_RECREATE_TARGET)
	{
		Utils::safe_release(&d2d1_rt_);
		Utils::safe_release(&d2d1_solidbrush_);
	}
}

void Editor::begin_draw()
{
	BeginPaint(hwnd_, &ps_);
	d2d1_rt_->BeginDraw();
}

void Editor::end_draw()
{
	EndPaint(hwnd_, &ps_);
	render_result_ = d2d1_rt_->EndDraw();
}

void Editor::update()
{
	if (in_menu)
	{
		maps_button.check_click(mouse_position_, mouse_type_);

		maps_button.set_button_color(Utils::create_d2d1_color(255, 255, 255, 255));
		maps_button.check_hover(mouse_position_);

		new_map_button.check_click(mouse_position_, mouse_type_);

		new_map_button.set_button_color(Utils::create_d2d1_color(255, 255, 255, 255));
		new_map_button.check_hover(mouse_position_);

		for (auto& pair : maps_)
		{
			pair.first.check_click(mouse_position_, mouse_type_);

			pair.first.set_button_color(Utils::create_d2d1_color(255, 255, 255, 255));
			pair.first.check_hover(mouse_position_);
		}

		mouse_type_ = mouse_type::NONE;
	}
	else
	{
		map_.save_button.set_button_color(Utils::create_d2d1_color(255, 255, 255, 255));
		map_.save_button.check_hover(mouse_position_);

		map_.menu_button.set_button_color(Utils::create_d2d1_color(255, 255, 255, 255));
		map_.menu_button.check_hover(mouse_position_);

		map_.on_type(&keys_);
		map_.on_click(mouse_position_, mouse_type_);
		map_.on_wheel(wheel_delta_);
		
		wheel_delta_ = 0;
	}

	std::fill(keys_.begin(), keys_.end(), std::pair<bool, int>(false, 0));
}

void Editor::render()
{
	if (in_menu)
	{
		d2d1_rt_->Clear(Utils::create_d2d1_color(180, 181, 254, 255));

		maps_button.draw(d2d1_rt_, d2d1_solidbrush_);
		new_map_button.draw(d2d1_rt_, d2d1_solidbrush_);

		for (auto& pair : maps_)
		{
			pair.first.draw(d2d1_rt_, d2d1_solidbrush_);
		}
	}
	else
	{
		d2d1_rt_->Clear(Utils::create_d2d1_color(255, 255, 255, 255));

		map_.draw(d2d1_rt_, d2d1_solidbrush_);
	}
}

void Editor::init_maps()
{
	maps_button.hide();
	new_map_button.hide();

	auto x = 70;
	auto y = 20;

	for (auto const& file : std::filesystem::directory_iterator("maps"))
	{
		auto path = file.path();

		if (path.extension() == ".dodgemap")
		{
			auto lower_name = path.stem().generic_wstring();
			Button button;

			button.init(dw_factory_);
			button.fixed_size(true);

			button.set_position(x, y);

			if (lower_name.length() > 8)
			{
				button.set_text(lower_name.substr(0, 8), dw_factory_);
			}
			else
			{
				button.set_text(lower_name, dw_factory_);
			}

			button.set_text_size(25.0f);
			button.set_size(175, 70);

			button.on_click = [this, &in_menu = in_menu, &dw_factory_ = dw_factory_, &maps_ = maps_](Button* sender, const mouse_type& button) {
				Utils::map_click(dw_factory_, &map_, sender, &maps_);
				in_menu = false;

				map_.menu_button.on_click = [this, &maps_ = maps_, &in_menu = in_menu](Button* sender, const mouse_type& button) {
					maps_.clear();

					this->maps_button.show();
					this->new_map_button.show();

					in_menu = true;
				};
			};
			button.on_hover = Utils::button_hover;

			maps_.emplace_back(button, lower_name);
			auto new_line = x + 215 >= 715;

			if (y + 90 > 380 && new_line) break;

			if (new_line)
			{
				x = 70;
				y += 90;
			}
			else
			{
				x += 215;
			}
		}
	}
}

