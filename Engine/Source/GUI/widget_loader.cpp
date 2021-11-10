#include "widget_loader.h"
#include "image_widget.h"
#include "text_widget.h"
#include <vector>
#include "3rdparty/tinyxml2/tinyxml2.h"
#include <codecvt>
#include "Debug/debug.h"
#include "GameEngine/game_engine.h"
#include "Texture/texture_loader.h"
#include <filesystem>
#include <memory>

namespace Ming3D
{
    struct WidgetLoadParams
    {
        std::string mFilePath;
        std::string mDirectory;
    };

    struct WidgetSizeValue
    {
        WidgetSizeMode mWidgetSizeMode = WidgetSizeMode::Absolute;
        float mValue = 0.0f;
    };

    std::vector<std::string> SplitString(const std::string text, const char delimiter)
    {
        std::vector<std::string> vec;
        size_t iStart = 0;
        size_t iCurr = 1;
        for (int iCurr = 0; iCurr < text.size(); iCurr++)
        {
            if (text[iCurr] == ' ')
            {
                std::string substr = text.substr(iStart, iCurr - iStart);
                vec.push_back(substr);
                iStart = iCurr + 1;
            }
        }
        if (iStart < text.size())
        {
            std::string substr = text.substr(iStart, text.size() - iStart);
            vec.push_back(substr);
        }
        return vec;
    }

    glm::vec4 ParseVec4(const std::string text)
    {
        glm::vec4 vec;
        std::vector<std::string> split = SplitString(text, ' ');

        for (int i = 0; i < 4 && i < split.size(); i++)
        {
            vec[i] = std::stof(split[i]);
        }
        return vec;
    }

    WidgetSizeValue ParseWidgetSizeValue(const std::string text)
    {
        WidgetSizeValue sizeVal;
        if (text.size() > 1 && text[text.size() - 1] == '%')
        {
            sizeVal.mWidgetSizeMode = WidgetSizeMode::Relative;
            sizeVal.mValue = std::stof(text.substr(0, text.size() - 1)) / 100.0f;
        }
        else if (text.size() > 2 && text.substr(text.size() - 2, 2) == "px")
        {
            sizeVal.mWidgetSizeMode = WidgetSizeMode::Absolute;
            sizeVal.mValue = std::stof(text.substr(0, text.size() - 2));
        }
        else
        {
            sizeVal.mWidgetSizeMode = WidgetSizeMode::Absolute;
            sizeVal.mValue = std::stof(text);
        }
        return sizeVal;
    }

    bool ParseHorizontalAlignment(std::string text, EHorizontalAlignment& outAlign)
    {
        if (std::strcmp(text.c_str(), "left") == 0)
            outAlign = EHorizontalAlignment::Left;
        else if (std::strcmp(text.c_str(), "centre") == 0)
            outAlign = EHorizontalAlignment::Centre;
        else if (std::strcmp(text.c_str(), "right") == 0)
            outAlign = EHorizontalAlignment::Right;
        else
            return false;

        return true;
    }

    bool ParseVerticalAlignment(std::string text, EVerticalAlignment& outAlign)
    {
        if (std::strcmp(text.c_str(), "top") == 0)
            outAlign = EVerticalAlignment::Top;
        else if (std::strcmp(text.c_str(), "centre") == 0)
            outAlign = EVerticalAlignment::Centre;
        else if (std::strcmp(text.c_str(), "bottom") == 0)
            outAlign = EVerticalAlignment::Bottom;
        else
            return false;

        return true;
    }

    Widget* ParseWidgetNode(tinyxml2::XMLElement* node, WidgetLoadParams params)
    {
        Widget* widget = nullptr;

        std::string nodeVal = node->Value();

        if (nodeVal == "CustomWidget")
        {
            widget = new Widget();
        }
        else if (nodeVal == "ImageWidget")
        {
            ImageWidget* imgWidget = new ImageWidget();
            const tinyxml2::XMLAttribute* colAttr = node->FindAttribute("colour");
            if (colAttr != nullptr)
                imgWidget->SetColour(ParseVec4(colAttr->Value()) / 255.0f);

            const tinyxml2::XMLAttribute* imgAttr = node->FindAttribute("image");
            if (imgAttr != nullptr)
            {
                const std::string texturePath = params.mDirectory + std::string("/") + std::string(imgAttr->Value());
                std::shared_ptr<Texture> texture = std::shared_ptr<Texture>(TextureLoader::LoadTextureData(texturePath)); // TODO: Re-use already imported textures
                imgWidget->SetTexture(texture);
            }

            widget = imgWidget;
        }
        else if (nodeVal == "TextWidget")
        {
            TextWidget* textWidget = new TextWidget();
            int fontSize = 24;
            const tinyxml2::XMLAttribute* fontSizeAttr = node->FindAttribute("font-size");
            if (fontSizeAttr != nullptr)
            {
                fontSize = std::stoi(fontSizeAttr->Value());
            }
            const tinyxml2::XMLAttribute* textAttr = node->FindAttribute("text");
            if (textAttr != nullptr)
            {
                textWidget->SetFont(GGameEngine->GetResourceDirectory() + std::string("/Fonts/FreeSans.ttf"), fontSize); // TODO!
                std::wstring_convert<std::codecvt_utf8_utf16<wchar_t>, wchar_t> conversion;
                std::wstring text = conversion.from_bytes(textAttr->Value());
                textWidget->SetText(text);
            }
            const tinyxml2::XMLAttribute* horAlignAttr = node->FindAttribute("horizontal-alignment");
            if (horAlignAttr != nullptr)
            {
                EHorizontalAlignment align;
                if (ParseHorizontalAlignment(horAlignAttr->Value(), align))
                    textWidget->SetHorizontalAlignment(align);
                else
                    LOG_ERROR() << "Invalid horizontal alignment value: " << horAlignAttr->Value();
            }
            const tinyxml2::XMLAttribute* vertAlignAttr = node->FindAttribute("vertical-alignment");
            if (vertAlignAttr != nullptr)
            {
                EVerticalAlignment align;
                if (ParseVerticalAlignment(vertAlignAttr->Value(), align))
                    textWidget->SetVerticalAlignment(align);
                else
                    LOG_ERROR() << "Invalid vertical alignment value: " << vertAlignAttr->Value();
            }
            widget = textWidget;
        }

        WidgetSizeValue xVal, yVal, wVal, hVal;
        const tinyxml2::XMLAttribute* xAttr = node->FindAttribute("x");
        const tinyxml2::XMLAttribute* yAttr = node->FindAttribute("y");
        const tinyxml2::XMLAttribute* wAttr = node->FindAttribute("width");
        const tinyxml2::XMLAttribute* hAttr = node->FindAttribute("height");
        if (xAttr != nullptr)
            xVal = ParseWidgetSizeValue(xAttr->Value());
        if (yAttr != nullptr)
            yVal = ParseWidgetSizeValue(yAttr->Value());
        if (wAttr != nullptr)
            wVal = ParseWidgetSizeValue(wAttr->Value());
        if (hAttr != nullptr)
            hVal = ParseWidgetSizeValue(hAttr->Value());

        widget->setHorizontalPositioning(xVal.mWidgetSizeMode);
        widget->setVerticalPositioning(yVal.mWidgetSizeMode);
        widget->setHorizontalScaling(wVal.mWidgetSizeMode);
        widget->setVerticalScaling(hVal.mWidgetSizeMode);
        widget->setPosition(xVal.mValue, yVal.mValue);
        widget->setSize(wVal.mValue, hVal.mValue);

        tinyxml2::XMLElement* currChild = node->FirstChildElement();
        while (currChild != nullptr)
        {
            Widget* childWidget = ParseWidgetNode(currChild, params);
            if (childWidget != nullptr)
                widget->addWidget(childWidget);
            currChild = currChild->NextSiblingElement();
        }
        return widget;
    }

    Widget* WidgetLoader::LoadWidgetFromXML(const std::string widgetPath)
    {
        WidgetLoadParams params;
        params.mFilePath = widgetPath;
        params.mDirectory = std::filesystem::path(widgetPath).parent_path().string();

        tinyxml2::XMLDocument doc;
        doc.LoadFile(widgetPath.c_str());

        tinyxml2::XMLElement* root = doc.RootElement();
        return ParseWidgetNode(root, params);
    }
}
