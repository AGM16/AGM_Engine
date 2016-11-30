#pragma once
#ifndef __PerfTimer_H__
#define __PerfTimer_H__

#include "p2Defs.h"

class PerfTimer
{
public:

	// Constructor
	PerfTimer();
	~PerfTimer();

	void Start_Real_Time();
	double ReadMs_RealTime() const;
	double ReadSeconds_RealTime() const;
	uint64 ReadTicks_RealTime() const;
	
	void Start_Game_Clock();
	double ReadMs_GameClock();
	double ReadSeconds_GameClock();
	uint64 ReadTicks_GameClock() const;

	void Update_GameClock_dt();

	void Play();
	void Stop();
	void Pause();

	bool Is_Game_Clock_Running()const;
	bool Is_Game_Clock_Paused()const;

	float Get_Game_Clock_Delta_Time()const;
	float Get_Frame_Count()const;
	float Get_Time_Scale()const;

	void Set_Time_Scale(const float& new_time_scale);

private:

	//Rela Time Clock (since the application starts)
	uint64	application_started_at = 0;
	static uint64 frequency;

	//Game Clock
	uint64	game_clock_started_at = 0;
	uint64	game_clock_paused = 0;
	float game_clock_dt = 0;
	bool running = false;
	bool game_paused = false;
	float time_game_clock = 0;

	float time_scale = 1.f;
	uint frame_count = 0;
	uint64	frame_started = 0;
	
};

#endif //__PerfTimer_H__
