// ----------------------------------------------------
// PerfTimer.cpp
// Slow timer with microsecond precision
// ----------------------------------------------------

#include "PerfTimer.h"
#include "SDL\include\SDL_timer.h"

uint64 PerfTimer::frequency = 0;

// ---------------------------------------------
PerfTimer::PerfTimer()
{
	if (frequency == 0)
		frequency = SDL_GetPerformanceFrequency();
	Start_Real_Time();
}

PerfTimer::~PerfTimer()
{

}

// ------------------REAL TIME---------------------------
void PerfTimer::Start_Real_Time()
{
	//Initialize Real_Time_Since_Startup
	application_started_at = SDL_GetPerformanceCounter();
}


double PerfTimer::ReadMs_RealTime() const
{
	return ((double)ReadTicks_RealTime() / (double)frequency) * 1000.0f;
}


double PerfTimer::ReadSeconds_RealTime() const
{
	return (double)ReadTicks_RealTime() / (double)frequency;
}


uint64 PerfTimer::ReadTicks_RealTime() const
{
	return SDL_GetPerformanceCounter() - application_started_at;
}

// ------------------------------------------------------

// ------------------REAL TIME---------------------------
void PerfTimer::Start_Game_Clock()
{
	//Initialize Game_Clock
	game_clock_started_at = SDL_GetPerformanceCounter();
}

uint64 PerfTimer::ReadTicks_GameClock() const
{
	if (running == true)
		return SDL_GetPerformanceCounter() - game_clock_started_at;
	else
		return SDL_GetPerformanceCounter() - game_clock_paused;
}

double PerfTimer::ReadMs_GameClock() const
{
	return ((double)ReadTicks_GameClock() / (double)frequency) * 1000.0f;
}

double PerfTimer::ReadSeconds_GameClock() const
{
	double time = 0;
	if (game_clock_started_at > 0)
	{
		time = (double(SDL_GetPerformanceCounter() - game_clock_started_at) / (double)frequency);
		
		if(running == false)
		{
			time -= (double(ReadTicks_GameClock()) / double(frequency));
		}
	}

	return time;
	
}


// ------------------------------------------------------

void PerfTimer::Play()
{
	running = true;
	Start_Game_Clock();
}

void PerfTimer::Stop()
{
	running = false;
	game_clock_started_at = 0;
}

void PerfTimer::Pause()
{
	running = false;
	game_clock_paused = SDL_GetPerformanceCounter();
}




