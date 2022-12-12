#pragma once

namespace Ming3D
{
    class Actor;

    class Editor
    {
    public:
        Editor();
        ~Editor();

        Actor* mSelectedActor = nullptr; // TODO: Smart pointer

    public:
        void Run();
    };
}

namespace Ming3D
{
    extern Editor* GEditor;
}
