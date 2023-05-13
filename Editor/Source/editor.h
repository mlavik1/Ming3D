#pragma once
#include "Object/object_ptr.h"

namespace Ming3D
{
    class Editor
    {
    public:
        Editor();
        ~Editor();

        ActorPtr mSelectedActor = nullptr; // TODO: Smart pointer

    public:
        void Run();
    };
}

namespace Ming3D
{
    extern Editor* GEditor;
}
