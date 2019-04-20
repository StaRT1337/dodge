#include "stdafx.hpp"
#include "Game.hpp"

LRESULT __stdcall window_proc(HWND hwnd, UINT msg, WPARAM w_param, LPARAM l_param)
{
	switch (msg)
	{
	case WM_CLOSE:
		DestroyWindow(hwnd);
		break;
	case WM_DESTROY:
		PostQuitMessage(0);
		break;
	case WM_PAINT:
	{
		PAINTSTRUCT ps;
		BeginPaint(hwnd, &ps);

		RECT rc;
		GetClientRect(
			hwnd,
			&rc
		);

		HGDIOBJ original = 0;
		original = SelectObject(ps.hdc, GetStockObject(DC_PEN));
		HPEN black_pen = CreatePen(PS_SOLID, 0, 3);

		SelectObject(ps.hdc, black_pen);

		Rectangle(
			ps.hdc,
			rc.left + 100,
			rc.top + 100,
			rc.right - 100,
			rc.bottom - 100);

		DeleteObject(black_pen);
		SelectObject(ps.hdc, original);

		EndPaint(hwnd, &ps);
		break;
	}
	default:
		return DefWindowProcA(hwnd, msg, w_param, l_param);
	}

	return 0;
}

int __stdcall WinMain(HINSTANCE instance, HINSTANCE prev_instance, char* cmd_line, int cmd_show)
{
	try
	{
		Game game;

		game.init(instance, prev_instance, cmd_line, cmd_show);
		game.run();
	}
	catch (const std::exception& exception)
	{
		std::ofstream log("logs.dlog", std::ios::app | std::ios::out);

		if (log)
		{
			auto time = std::time(0);
			auto local_time = std::localtime(&time);

			auto str = fmt::format("{}/{}/{}, {}:{}:{} {}",
				local_time->tm_mday,
				local_time->tm_mon,
				local_time->tm_year - 100,
				local_time->tm_hour,
				local_time->tm_min,
				local_time->tm_sec,
				exception.what());

			log.write(str.data(), str.size());
		}

		return 1;
	}

	/*WNDCLASSEX wc;

	wc.cbSize = sizeof(WNDCLASSEX);
	wc.style = 0;
	wc.lpfnWndProc = window_proc;
	wc.cbClsExtra = 0;
	wc.cbWndExtra = 0;
	wc.hInstance = instance;
	wc.hIcon = LoadIconA(0, IDI_APPLICATION);
	wc.hCursor = LoadCursorA(0, IDC_ARROW);
	wc.hbrBackground = reinterpret_cast<HBRUSH>(COLOR_WINDOW + 1);
	wc.lpszMenuName = 0;
	wc.lpszClassName = "main_dodge_window";
	wc.hIconSm = LoadIconA(0, IDI_APPLICATION);

	if (!RegisterClassExA(&wc))
	{
		MessageBoxA(0, "Couldn't register class", 0, MB_OK);
	}

	auto hwnd = CreateWindowExA(
		WS_EX_CLIENTEDGE,
		"main_dodge_window",
		"dodge",
		WS_OVERLAPPED | WS_CAPTION | WS_SYSMENU | WS_MINIMIZEBOX,
		CW_USEDEFAULT, CW_USEDEFAULT,
		750, 510, 0, 0, instance, 0
	);

	ShowWindow(hwnd, cmd_show);
	UpdateWindow(hwnd); */

	return 0;
}
