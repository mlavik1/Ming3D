#pragma once
#include <string>

namespace Ming3D
{
    class Widget;

    class WidgetLoader
    {
    public:
        static Widget* LoadWidgetFromXML(const std::string widgetPath);
    };
}
