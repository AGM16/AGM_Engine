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

	//Starts the application realtime clock
	Start_Real_Time();
	new_frame_started = application_started_at;
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

// ------------------Game Clock TIME---------------------------
void PerfTimer::Start_Game_Clock()
{
	//Initialize Game_Clock
	game_clock_started_at = SDL_GetPerformanceCounter();
}

uint64 PerfTimer::ReadTicks_GameClock() const
{
	if (running == true && !game_clock_paused)
		return SDL_GetPerformanceCounter() - game_clock_started_at;
	else
		return SDL_GetPerformanceCounter() - game_clock_paused;
}

double PerfTimer::ReadMs_GameClock() 
{
	if (game_clock_started_at > 0)
	{
		float game_time_since_start_at = (double(SDL_GetPerformanceCounter() - game_clock_started_at) / (double)frequency)* 1000.f;

		if (running == true && game_clock_started_at > 0)
		{
			time_game_clock = game_time_since_start_at * time_scale;
			++frame_count;
		}
		else
		{
			if (game_paused)
			{
				game_time_since_start_at -= ((double)ReadTicks_GameClock() / (double)frequency)* 1000.f;
				time_game_clock = game_time_since_start_at * time_scale;
			}
		}
	}

	return time_game_clock;
}

double PerfTimer::ReadSeconds_GameClock() 
{
	if (game_clock_started_at > 0)
	{
		float game_time_since_start_at = (double(SDL_GetPerformanceCounter() - game_clock_started_at) / (double)frequency);

		if (running == true && game_clock_started_at > 0)
		{
			time_game_clock = game_time_since_start_at * time_scale;
			++frame_count;
		}
		else
		{
			if (game_paused)
			{
				game_time_since_start_at -= ((double)ReadTicks_GameClock() / (double)frequency);
				time_game_clock = game_time_since_start_at * time_scale;
			}
		}

	}

	return time_game_clock;

}

void PerfTimer::Update_GameClock_dt()
{
	game_clock_dt = (double(SDL_GetPerformanceCounter() - new_frame_started) / (double)frequency);
	new_frame_started = SDL_GetPerformanceCounter();
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
	time_game_clock = 0;
}

void PerfTimer::Pause()
{
	running = false;
	game_paused = true;
	game_clock_paused = SDL_GetPerformanceCounter();
}


bool PerfTimer::Is_Game_Clock_Running()const
{
	return running;
}

bool PerfTimer::Is_Game_Clock_Paused()const
{
	return game_paused;
}

float PerfTimer::Get_Game_Clock_Delta_Time()const
{
	return game_clock_dt;
}

float PerfTimer::Get_Frame_Count()const
{
	return frame_count;
}

float PerfTimer::Get_Time_Scale()const
{
	return time_scale;
}

void PerfTimer::Set_Time_Scale(const float& new_time_scale)
{
	if(new_time_scale > 0.1f)
		time_scale = new_time_scale;
}





