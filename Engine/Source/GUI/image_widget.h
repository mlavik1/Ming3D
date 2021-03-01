#include "widget.h"
#include "glm/vec4.hpp"

namespace Ming3D
{
    class ImageVisual;
    class Texture;

    class ImageWidget : public Widget
    {
    private:
        ImageVisual* mImageVisual;

    public:
        ImageWidget();
        ~ImageWidget();

        void SetColour(glm::vec4 colour);
        void SetTexture(Texture* texture);
    };
}
