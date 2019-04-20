#include "stdafx.hpp"
#include "Game.hpp"

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

	return 0;
}
