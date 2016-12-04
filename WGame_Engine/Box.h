#include "MathGeoLib\include\MathGeoLib.h"

class Box
{
public:
	math::AABB rect;
	math::float2 centre;
	math::float2 size;

public:
	Box(math::float2 centre_value, math::float2 Size_value) : centre(centre_value), size(Size_value)
	{
		rect.SetFromCenterAndSize(math::vec(centre_value.x, 10.f, centre_value.y), vec(size.x, 20.f, size.y));
	};

	bool contains(const math::float2 pos_go) const
	{
		if (pos_go.x < centre.x + (size.x / 2) && pos_go.x > centre.x - (size.x / 2))
		{
			if (pos_go.y < centre.y + (size.y/2) && pos_go.y > centre.y - (size.y / 2))
			{
				return true;
			}
		}
		return false;
	}

	math::float2 Get_Center()const
	{
		return centre;
	}

	math::float2 Get_Size()const
	{
		return size;
	}

	void Clear()
	{
		centre = float2::zero;
		size = float2::zero;
	}
};

