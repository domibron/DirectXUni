#pragma once

#include <chrono>

class TimeKeeping
{
private:
	std::chrono::time_point<std::chrono::high_resolution_clock> start;
	std::chrono::time_point<std::chrono::high_resolution_clock> end;

	float deltaTime = 0.0f;

public:
	TimeKeeping();
	void Tick();
	float GetDeltaTime();
};

