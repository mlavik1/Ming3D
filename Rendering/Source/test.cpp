#include "test.h"

#include "Debug/debug.h"

#include <Windows.h>

#include <gl/glew.h>

#include <SDL.h>
//#include <SDL_opengl.h>


namespace Ming3D
{
	void RenderingTest::Test()
	{
		if (SDL_Init(SDL_INIT_VIDEO) != 0)
			LOG_ERROR() << "Failed to initialise SDL";
		else
		{
			SDL_version linkedver; SDL_version compiledver;
			SDL_GetVersion(&linkedver);
			SDL_VERSION(&compiledver);
			LOG_INFO() << "SDL compiled version: " << (int)compiledver.major << "." << (int)compiledver.minor << ", pathch: " << (int)compiledver.patch;
			LOG_INFO() << "SDL linked version: " << (int)linkedver.major << "." << (int)linkedver.minor << ", pathch: " << (int)linkedver.patch;
		}

		SDL_Window* mMainWindow = SDL_CreateWindow("Ming3D", 0, 0, 800, 600, SDL_WINDOW_SHOWN | SDL_WINDOW_OPENGL);
		SDL_GLContext mMainGLContext = SDL_GL_CreateContext(mMainWindow);
		
		LOG_INFO() << "Using OpenGL, version " << glGetString(GL_VERSION);

		glClearColor(1.0f, 0.0f, 0.0f, 1.0f);

		glClear(GL_COLOR_BUFFER_BIT);

		SDL_GL_SwapWindow(mMainWindow);
	}
}
