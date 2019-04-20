#pragma once

using namespace std::chrono;

class Timer
{
private:
	time_point<high_resolution_clock> prev_time;
public:
	Timer();
	float get_delta();
};
