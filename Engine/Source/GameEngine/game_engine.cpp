#include "game_engine.h"

#include "Source/ClassManager/class_manager.h"

namespace Ming3D
{
	GameEngine::GameEngine()
	{
		mClassManager = new ClassManager();
	}

	GameEngine::~GameEngine()
	{
		delete mClassManager;
	}

	void GameEngine::Initialise()
	{
		mClassManager->InitialiseClasses();
	}
}
