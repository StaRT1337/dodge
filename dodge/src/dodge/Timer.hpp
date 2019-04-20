#pragma once

using namespace std::chrono;

class Timer
{
private:
	time_point<high_resolution_clock> prev_time;
public:
	Timer() : prev_time(high_resolution_clock::now()) {};
	float get_delta();
};
