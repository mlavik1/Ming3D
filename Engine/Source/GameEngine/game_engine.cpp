#include "game_engine.h"

#include "Source/ClassManager/class_manager.h"
#include "renderingtest.h"

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
	
		RenderingTest rt;
		rt.Test();
	}
}
