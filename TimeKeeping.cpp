#include "TimeKeeping.h"

TimeKeeping::TimeKeeping()
{
	start = std::chrono::high_resolution_clock::now();
	Tick(); // get a tick going.
}

// Ticks the time, call this after your render frame, this timing render speed.
void TimeKeeping::Tick()
{
	end = std::chrono::high_resolution_clock::now();
	std::chrono::duration<float> elapsed_seconds = end - start;

	deltaTime = elapsed_seconds.count();
	start = std::chrono::high_resolution_clock::now();

	// should be a begin and end tick system rather than a single tick but for ease of use, this will be like this.
	// just a note incase any bugs with time occur, if this comment never gets adressed, then current solution suffices.
}

float TimeKeeping::GetDeltaTime()
{
	return deltaTime;
}
