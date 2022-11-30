#ifndef MING3D_PROEPRTYINSPECTORBUILDER_H
#define MING3D_PROEPRTYINSPECTORBUILDER_H

#include <functional>
#include <vector>
#include "control.h"
#include "button.h"
#include "text_box.h"
#include "Object/property.h"
#include "Object/object.h"
#include <set>

#define MING3D_FLOATFIELD_HASH 1
#define MING3D_INTFIELD_HASH 2
#define MING3D_BUTTON_HASH 3


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
    typedef uint64_t ctrl_id;

    struct EditorControlStyle
    {
    public:
        int fontSize = 12;
        // TODO: bold, italic, font (use font resource ID?)
        // TODO: states: default, hover, pressed, disabled
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

    class WindowLayoutBuilder
    {
    private:
        NativeUI::Control* mParentControl;

        EditorControlStyle mDefaultStyle;

        int mCurrVertIndex = 0;
        int mCurrHorIndex = 0;
        float mCurrHeight = 0.0f;

        const float ctrlHeight = 20.0f;
        const float vertPadding = 10.0f;

        bool mHorizontalMode = false;

        // key: vertical and horizontal index
        std::unordered_map<ctrl_id, int> mControlIDs;
        std::unordered_map<ctrl_id, NativeUI::Control*> mControls;
        std::unordered_map<ctrl_id, CachedControlData> mControlDataCache;
        std::set<ctrl_id> mControlsToUpdate; // keeps track of which controls we have updated (so we can delete the ones we no longer use)

        std::vector<NativeUI::Control*> mQueueHorizontalControls;

        ctrl_id GetControlKeyHash(int typeHash, int verticalIndex, int horizontalIndex);
        NativeUI::Control* GetControl(ctrl_id controlKeyHash);
        std::string TextFieldInternal(std::string inText, uint64_t controlKeyHash, bool readOnly, const EditorControlStyle &inStyle);

    public:
        WindowLayoutBuilder(NativeUI::Control* inControl);

        void Space();
        
        bool BoolField(bool inValue, const EditorControlStyle &inStyle);
        bool BoolField(bool inValue);
        int IntField(int inValue, const EditorControlStyle &inStyle);
        int IntField(int inValue);
        float FloatField(float inValue, const EditorControlStyle &inStyle);
        float FloatField(float inValue);
        std::string TextField(const std::string& inText, const EditorControlStyle &inStyle);
        std::string TextField(const std::string& inText);
        void LabelField(const std::string& inText, const EditorControlStyle &inStyle);
        void LabelField(const std::string& inText);
        bool Button(const std::string& inText, const EditorControlStyle &inStyle);
        bool Button(const std::string& inText);

        void PropertyField(Property* inProp, Object* inObject, std::string inName);

        void BeginHorizontal();
        void EndHorizontal();

        void BeginBuilding();
        void EndBuilding();
    };
}

#endif
