#ifndef MING3D_GAMEENGINE_H
#define MING3D_GAMEENGINE_H

namespace Ming3D
{
	class ClassManager;

	class GameEngine
	{
	private:
		ClassManager* mClassManager;

	public:
		GameEngine();
		~GameEngine();

		void Initialise();
	};
}

#endif
