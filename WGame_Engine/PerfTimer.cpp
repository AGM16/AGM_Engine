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
	double time_game_clock = 0;
	if (game_clock_started_at > 0)
	{
		time_game_clock = (double(SDL_GetPerformanceCounter() - game_clock_started_at) / (double)frequency)* 1000.0f;

		if (running == false)
		{
			time_game_clock -= ((double)ReadTicks_GameClock() / (double)frequency) * 1000.0f;
		}
	}

	return time_game_clock;
}

double PerfTimer::ReadSeconds_GameClock() const
{
	double time_game_clock = 0;
	if (game_clock_started_at > 0)
	{
		time_game_clock = (double(SDL_GetPerformanceCounter() - game_clock_started_at) / (double)frequency);
		
		if(running == false)
		{
			time_game_clock -= (double(ReadTicks_GameClock()) / double(frequency));
		}
	}

	return time_game_clock;	
}


// ------------------------------------------------------

void PerfTimer::Play()
{
	//It means that we had paused the game
	if (running == false && game_paused == true)
	{
        running = true;
		game_paused == false;
	}
	else
	{	
		//When we press play at the beging
		Start_Game_Clock();
		if(!running)
			running = true;
	}
}

void PerfTimer::Stop()
{
	running = false;
	game_paused = false;
	game_clock_started_at = 0;
}

void PerfTimer::Pause()
{
	running = false;
	game_paused = true;
	game_clock_paused = SDL_GetPerformanceCounter();
}




