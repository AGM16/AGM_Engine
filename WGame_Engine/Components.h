#ifndef _COMPONENTS_H_
#define _COMPONENTS_H_

enum Components_Type
{
	TRANSFORMATION,
	MESH,
	MATERIAL,
	UNKOWN
};


class Components
{
public:

	Components(Components_Type type);
	~Components();

	bool Enable();
	virtual void Update();
	bool Disable();

	bool Is_Active()const;
	Components_Type Get_Type()const;

private:

	Components_Type type_c;
	bool active_component;

};

#endif // !_COMPONENTS_H_

