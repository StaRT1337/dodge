#include "stdafx.hpp"
#include "Timer.hpp"

Timer::Timer()
{
	prev_time = high_resolution_clock::now();
}

float Timer::get_delta()
{
	auto current_time = high_resolution_clock::now();

	duration<float> delta(current_time - prev_time);
	prev_time = high_resolution_clock::now();

	return delta.count();
}
