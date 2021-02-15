#pragma once

namespace Ming3D
{
    namespace Rendering
    {
        class RenderWindow;
        class RenderTarget;
    }

    class RenderWindowHandle
    {
    public:
        Rendering::RenderWindow* mRenderWindow;
        Rendering::RenderTarget* mRenderTarget;
    };
}
