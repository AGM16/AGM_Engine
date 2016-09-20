#include "PhysVehicle3D.h"
#include "Primitive.h"
#include "Bullet\include\btBulletDynamicsCommon.h"
#include "MathGeoLib\include\MathGeoLib.h"

// ----------------------------------------------------------------------------
VehicleInfo::~VehicleInfo()
{
	//if(wheels != NULL)
	//delete wheels;
}

// ----------------------------------------------------------------------------
PhysVehicle3D::PhysVehicle3D(btRigidBody* body, btRaycastVehicle* vehicle, const VehicleInfo& info) : PhysBody3D(body), vehicle(vehicle), info(info)
{
}

// ----------------------------------------------------------------------------
PhysVehicle3D::~PhysVehicle3D()
{
	delete vehicle;
}

// ----------------------------------------------------------------------------
void PhysVehicle3D::Render()
{
	Primitive_Cylinder wheel;

	wheel.color = Green;

	for (int i = 0; i < vehicle->getNumWheels(); ++i)
	{
		wheel.radius = info.wheels[0].radius;
		wheel.height = info.wheels[0].width;

		vehicle->updateWheelTransform(i);
		vehicle->getWheelInfo(i).m_worldTransform.getOpenGLMatrix(*wheel.transform.v);

		wheel.Render();
	}

	Primitive_Cube chassis(info.chassis_size.x, info.chassis_size.y, info.chassis_size.z);
	vehicle->getChassisWorldTransform().getOpenGLMatrix(*chassis.transform.v);
	btQuaternion q = vehicle->getChassisWorldTransform().getRotation();
	btVector3 offset(info.chassis_offset.x, info.chassis_offset.y, info.chassis_offset.z);
	offset = offset.rotate(q.getAxis(), q.getAngle());

	chassis.SetPos(offset.getX(), offset.getY(), offset.getZ());

	/*chassis.transform.M[12] += offset.getX();
	chassis.transform.M[13] += offset.getY();
	chassis.transform.M[14] += offset.getZ();*/

	chassis.Render();

	//--------------------------------------------------------------------------------

	Primitive_Cube chassis2(1, 1, 2);
	vehicle->getChassisWorldTransform().getOpenGLMatrix(*chassis2.transform.v);
	btQuaternion q2 = vehicle->getChassisWorldTransform().getRotation();
	btVector3 offset2(0, info.chassis_offset.y + 1.7f, -2.5f);
	offset2 = offset2.rotate(q2.getAxis(), q2.getAngle());

	chassis2.SetPos(offset2.getX(), offset2.getY(), offset2.getZ());

	/*chassis2.transform.M[12] += offset2.getX();
	chassis2.transform.M[13] += offset2.getY();
	chassis2.transform.M[14] += offset2.getZ();*/

	chassis2.Render();

}

// ----------------------------------------------------------------------------
void PhysVehicle3D::ApplyEngineForce(float force)
{
	for (int i = 0; i < vehicle->getNumWheels(); ++i)
	{
		if (info.wheels[i].drive == true)
		{
			vehicle->applyEngineForce(force, i);
		}
	}
}

// ----------------------------------------------------------------------------
void PhysVehicle3D::Brake(float force)
{
	for (int i = 0; i < vehicle->getNumWheels(); ++i)
	{
		if (info.wheels[i].brake == true)
		{
			vehicle->setBrake(force, i);
		}
	}
}

// ----------------------------------------------------------------------------
void PhysVehicle3D::Turn(float degrees)
{
	for (int i = 0; i < vehicle->getNumWheels(); ++i)
	{
		if (info.wheels[i].steering == true)
		{
			vehicle->setSteeringValue(degrees, i);
		}
	}
}

// ----------------------------------------------------------------------------
float3 PhysVehicle3D::GetForwardVector() const
{
	btVector3 h = vehicle->getForwardVector();
	float3 ret;
	ret.Set(h.getX(), h.getY(), h.getZ());
	return ret;
}

// ----------------------------------------------------------------------------
float PhysVehicle3D::GetKmh() const
{
	return vehicle->getCurrentSpeedKmHour();
}

float3 PhysVehicle3D::GetRefVehicle() const
{
	float3 pos;
	btTransform trans = vehicle->getChassisWorldTransform();
	btMatrix3x3 mat = trans.getBasis();
	pos.x = mat.getColumn(0).getX();
	pos.y = mat.getColumn(1).getY();
	pos.z = mat.getColumn(2).getZ();

	return pos;
}

void PhysVehicle3D::Orient(float rot_angle)
{
	float matrix[16];
	memset(matrix, 0.0f, sizeof(matrix));

	float3 p = GetPos();
	matrix[12] = p.x;
	matrix[13] = p.y;
	matrix[14] = p.z;
	matrix[15] = 1;

	matrix[0] = cos(rot_angle);
	matrix[2] = -sin(rot_angle);
	matrix[5] = 1;
	matrix[8] = sin(rot_angle);
	matrix[10] = cos(rot_angle);

	SetTransform(matrix);

}