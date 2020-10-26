#pragma once
#include "GameObject3D.h"
class ObjectKatamaria : public GameObject3D
{
public:
	float radius;
	bool canChild = true;
	bool justChild=false;
	float size;
	ObjectKatamaria() 
	{
		justChild = false;
	};
	typeObject GetType() override
	{
		return actor;
	}
};
