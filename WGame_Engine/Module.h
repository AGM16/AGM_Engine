#ifndef _MODULE_H_
#define _MODULE_H_

#include "PugiXml\src\pugixml.hpp"

class Application;
struct PhysBody3D;

class Module
{
private :
	bool enabled;
	std::string name;

public:
	

	Module(bool start_enabled = true)
	{}

	virtual ~Module()
	{}

	virtual bool Init() 
	{
		return true; 
	}

	virtual bool Start()
	{
		return true;
	}

	virtual update_status PreUpdate(float dt)
	{
		return UPDATE_CONTINUE;
	}

	virtual update_status Update(float dt)
	{
		return UPDATE_CONTINUE;
	}

	virtual update_status PostUpdate(float dt)
	{
		return UPDATE_CONTINUE;
	}

	virtual bool CleanUp() 
	{ 
		return true; 
	}

	virtual bool Load(pugi::xml_node& node)
	{
		return true;
	}

	virtual bool Save(pugi::xml_node& node) const
	{
		return true;
	}

	virtual void OnCollision(PhysBody3D* body1, PhysBody3D* body2)
	{}

	const char* Get_Name_Module()const
	{
		return name.c_str();
	}

	void Set_Name_Module(const char* new_name)
	{
		name = new_name;
	}
};

#endif