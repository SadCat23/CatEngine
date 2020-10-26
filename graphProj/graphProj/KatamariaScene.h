#pragma once
#include "GameObject3D.h"
class KatamariaScene : public GameObject3D
{
	typeObject GetType() override
	{
		return actor;
	}


};

