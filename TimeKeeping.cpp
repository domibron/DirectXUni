#include "TimeKeeping.h"

TimeKeeping::TimeKeeping()
{
	start = std::chrono::high_resolution_clock::now();
}

void TimeKeeping::Tick()
{
	end = std::chrono::high_resolution_clock::now();
	std::chrono::duration<float> elapsed_seconds = end - start;

	deltaTime = elapsed_seconds.count();
	start = std::chrono::high_resolution_clock::now();
}

float TimeKeeping::GetDeltaTime()
{
	return deltaTime;
}
