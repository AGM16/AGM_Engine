#include "MathGeoLib\include\MathGeoLib.h"

#define SIZE_Y_BOX 20.f
#define CENTRE_POS_Y_BOX 10.f

class Box
{
public:
	math::AABB rect;
	math::float2 centre;
	math::float2 size;

public:
	Box(math::float2 centre_value, math::float2 Size_value) : centre(centre_value), size(Size_value)
	{
		rect.SetFromCenterAndSize(math::vec(centre_value.x, CENTRE_POS_Y_BOX, centre_value.y), vec(size.x, SIZE_Y_BOX, size.y));
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

	void Set_Boundaries_rect(const math::float2 size_rect)
	{
		rect.SetFromCenterAndSize(math::vec(centre.x, 10.f, centre.y), vec(size_rect.x, 20.f, size_rect.y));
		size = size_rect;
	}

	void Clear()
	{
		centre = float2::zero;
		size = float2::zero;
		rect.SetNegativeInfinity();
	}
};

