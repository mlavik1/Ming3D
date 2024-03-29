#ifndef MING3D_WINDOW_BASE_H
#define MING3D_WINDOW_BASE_H

namespace Ming3D::Rendering
{
    class WindowBase
    {
    public:
        virtual ~WindowBase() = default;
        virtual void Initialise() = 0;
        virtual void SetSize(unsigned int inWidth, unsigned int inHeight) = 0;
        virtual unsigned int GetWidth() const = 0;
        virtual unsigned int GetHeight() const = 0;
        virtual void BeginRender() = 0;
        virtual void EndRender() = 0;
        virtual void* GetOSWindowHandle() = 0;
        virtual void Close() = 0;
        virtual bool IsOpen() = 0;
    };
}

#endif
