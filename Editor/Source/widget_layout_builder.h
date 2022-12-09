#pragma once

#include <functional>
#include <vector>
#include "Object/property.h"
#include "Object/object.h"
#include <set>
#include <memory>
#include <unordered_map>
#include "GUI/widget_transform.h"

#define MING3D_FLOATFIELD_HASH 1
#define MING3D_INTFIELD_HASH 2
#define MING3D_BUTTON_HASH 3
#define MING3D_TEXTFIELD_HASH 3
#define MING3D_LABEL_HASH 3

// !!!!!
// !!!!!
// TODO: 
//   - Optimise Horisontal mode: Don't update positions every frame (see use of mQueueHorizontalControls)
//   - Toggle boxes
//   - Update control position/sizee when needed
//   - Optimize: Don't do conversion between float-string-float etc. unless we actually need to (check for modificiations first)
//         if(CachedControlData.updatedByUser || inVal != CachedControlData.floatValue)


namespace Ming3D
{
    class Widget;

    typedef uint64_t widget_id;

    struct EditorControlStyle
    {
    public:
        int fontSize = 16;
        // TODO: bold, italic, font (use font resource ID?)
        // TODO: states: default, hover, pressed, disabled
        // TODO: min/max size, etc.
    };

    // TODO: move this to another file?
    struct CachedControlData
    {
        //bool boolValue = 0;
        //int intValue = 0;
        //float floatValue = 0.0f;
        std::string stringValue = "";
        bool modifiedByUser = false;
        EditorControlStyle style;
    };

    class WidgetLayoutBuilder
    {
    private:
        Widget* mContainerWidget;
        WidgetRect mContainerRect;

        EditorControlStyle mDefaultStyle;

        int mCurrVertIndex = 0;
        int mCurrHorIndex = 0;
        float mCurrHeight = 0.0f;

        const float mWidgetheight = 20.0f;
        const float mVertPadding = 10.0f;

        bool mHorizontalMode = false;

        // key: vertical and horizontal index
        std::unordered_map<widget_id, int> mWidgetIDs;
        std::unordered_map<widget_id, std::shared_ptr<Widget>> mWidgets;
        std::unordered_map<widget_id, CachedControlData> mWidgetDataCache;
        std::set<widget_id> mWidgetsToUpdate; // keeps track of which controls we have updated (so we can delete the ones we no longer use)

        std::vector<std::shared_ptr<Widget>> mQueueHorizontalWidgets;

        widget_id GetWidgetKeyHash(int typeHash, int verticalIndex, int horizontalIndex);
        std::shared_ptr<Widget> GetWidget(widget_id controlKeyHash);
        void LabelInternal(std::string inText, uint64_t controlKeyHash, bool readOnly, const EditorControlStyle &style);
        std::string TextFieldInternal(std::string inText, uint64_t controlKeyHash, bool readOnly, const EditorControlStyle &style);

    public:
        WidgetLayoutBuilder(Widget* containerWidget);

        void Space();
        
        bool BoolField(bool value, const EditorControlStyle &style);
        bool BoolField(bool value);
        int IntField(int value, const EditorControlStyle &style);
        int IntField(int value);
        float FloatField(float value, const EditorControlStyle &style);
        float FloatField(float value);
        std::string TextField(const std::string& inText, const EditorControlStyle &style);
        std::string TextField(const std::string& inText);
        void Label(const std::string& inText, const EditorControlStyle &style);
        void Label(const std::string& inText);
        bool Button(const std::string& inText, const EditorControlStyle &style);
        bool Button(const std::string& inText);

        void PropertyField(Property* propert, Object* object, std::string name);

        void BeginHorizontal();
        void EndHorizontal();

        void BeginBuilding();
        void EndBuilding();
    };
}
