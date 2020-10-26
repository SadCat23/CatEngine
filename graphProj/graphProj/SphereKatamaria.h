#pragma once
#include "ObjectKatamaria.h"
#include <vector>
class SphereKatamaria: public GameObject3D
{
public:
	
	float size;
	typeObject GetType() override
	{
		return actor;
	}

};