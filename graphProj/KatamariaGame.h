#pragma once
#include "Engine.h"
class KatamariaGame
{
public:
	void Init();

	void SpawnObject();

	void GameLoop();

	bool CheckCollison(ObjectKatamaria* obj);

	void Input(char Key);
	
	void AddToChild(ObjectKatamaria* obj);

	

	static KatamariaGame& getInstance() {
		static KatamariaGame  GameInstance;
		return GameInstance;
	}
	
	Engine* MyUrealEngine;
	KatamariaScene* scene;
	SphereKatamaria* sphere;
	std::vector <ObjectKatamaria*> objects;
private:

	KatamariaGame() {};
	KatamariaGame(const KatamariaGame&);


};



