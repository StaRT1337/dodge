#include "stdafx.hpp"
#include "include/Timer.hpp"

float Timer::get_delta()
{
	auto current_time = high_resolution_clock::now();

	duration<float> delta(current_time - prev_time_);
	prev_time_ = high_resolution_clock::now();

	return delta.count();
}
