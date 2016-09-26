#ifndef _FPS_
#define _FPS_

#include "ModuleInfoWindow.h"
#include "MathGeoLib\include\MathGeoLib.h"


class FPS : public ModuleInfoWindow
{
public:

	FPS();
	~FPS();

	void Render_FPS();

private:

	float3 frames;
	int max_frames;

};





#endif