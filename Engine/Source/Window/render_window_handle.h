#pragma once

#include <memory>

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
        Rendering::RenderWindow* mRenderWindow; // TODO:
        std::shared_ptr<Rendering::RenderTarget> mRenderTarget;
    };
}
