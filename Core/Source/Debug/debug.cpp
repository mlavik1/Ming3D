#include "debug.h"

namespace Ming3D
{
	DEBUG_MODE Debug::outputMode = DEBUG_MODE_NONE;
	DEBUG_MODE Debug::terminalLogMode = DEBUG_MODE_ALL;
	DEBUG_MODE Debug::fileLogMode = DEBUG_MODE_ERROR;

	bool Debug::firstTime = true;
}
