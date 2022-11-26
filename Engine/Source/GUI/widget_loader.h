#pragma once
#include <string>
#include <memory>

namespace Ming3D
{
    class Widget;

    class WidgetLoader
    {
    public:
        static std::shared_ptr<Widget> LoadWidgetFromXML(const std::string& widgetPath);
    };
}
