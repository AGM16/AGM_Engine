#include "Component_Billboard.h"
#include "Components.h"
#include "Component_Transformation.h"
#include "GameObject.h"

using namespace std;

Component_Billboard::Component_Billboard(Components_Type type, GameObject* game_object) : Components(type, game_object)
{

}

Component_Billboard::~Component_Billboard()
{

}

void Component_Billboard::Update_BillBoard(const float3 look_to_point, const float3 up_vector)
{
	if (Get_Game_Object()->Get_Component(TRANSFORMATION) != nullptr)
	{
		Component_Transformation* transform_go = (Component_Transformation*)Get_Game_Object()->Get_Component(TRANSFORMATION);

		if (up_vector.IsZero() == false)
		{

			float3 forward = look_to_point - transform_go->Get_Tranformation_Matrix().TranslatePart().Normalized();

			float4x4 tmp = float4x4::LookAt(localForward, forward.Normalized(), localUp, up_vector.Normalized());

			transform_go->Set_Rotation(RadToDeg(tmp.ToEulerXYZ()));
		}
	}
}
