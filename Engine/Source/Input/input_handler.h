#ifndef MING3D_INPUTHANDLER_H
#define MING3D_INPUTHANDLER_H

namespace Ming3D
{
    class InputHandler
    {
    public:
        virtual ~InputHandler() {}
        virtual void Initialise() = 0;
        virtual void Update() = 0;
    };
}

#endif
