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
	double ReadMs_GameClock() const;
	double ReadSeconds_GameClock() const;
	uint64 ReadTicks_GameClock() const;

	void Play();
	void Stop();
	void Pause();

private:

	//Rela Time Clock (since the application starts)
	uint64	application_started_at = 0;
	static uint64 frequency;

	//Game Clock
	uint64	game_clock_started_at = 0;
	uint64	game_clock_paused = 0;
	bool running = false;

	
};

#endif //__PerfTimer_H__
