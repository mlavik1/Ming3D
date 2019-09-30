#include "debug_stats.h"

namespace Ming3D
{
    DebugStatController<int>* GStatControllerInt = new DebugStatController<int>();
    DebugStatController<float>* GStatControllerFloat = new DebugStatController<float>();
    DebugStatController<int>* GFrameStatControllerInt = new DebugStatController<int>();
    DebugStatController<float>* GFrameStatControllerFloat = new DebugStatController<float>();
}
