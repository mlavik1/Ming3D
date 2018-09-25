#ifndef NATIVEUI_CONTROL
#define NATIVEUI_CONTROL

#include "types.h"
#include <Windows.h>
#include <vector>

namespace NativeUI
{
	class Control
	{
	protected:
		Control* mParent;
		std::vector<Control*> mChildren;
		Point mPosition;
		Point mSize;
		SizeMode mVerticalSizeMode = SizeMode::Absolute;
		SizeMode mHorizontalSizeMode = SizeMode::Absolute;
		SizeMode mVerticalPositionMode = SizeMode::Absolute;
		SizeMode mHorizontalPositionMode = SizeMode::Absolute;
		HWND mHwnd = NULL;

		Point mPositionClientCoords;
		Point mSizeClientCoords;

		void UpdateClientCoords();
		virtual void OnTransformUpdated();
		virtual void OnChildTransformUpdated();

		void OnUserResize();

	public:
		Control(Control* arg_parent);
        virtual ~Control() {}

		void SetPosition(const Point& arg_pos);
		void SetSize(const Point& arg_size);
		void SetPosition(const float& arg_x, const float& arg_y);
		void SetSize(const float& arg_w, const float& arg_h);
		void SetVerticalSizeMode(const SizeMode& arg_sizemode);
		void SetHorizontalSizeMode(const SizeMode& arg_sizemode);
		void SetVerticalPositionMode(const SizeMode& arg_sizemode);
		void SetHorizontalPositionMode(const SizeMode& arg_sizemode);

		virtual void OnResize();
		virtual void OnParentResize();

		inline Point GetPosition() { return mPosition; }
		inline Point GetSize() { return mSize; }
		inline SizeMode GetVerticalSizeMode() { return mVerticalSizeMode; }
		inline SizeMode GetHorizontalSizeMode() { return mHorizontalSizeMode; }
		inline HWND GetHwnd() { return mHwnd; }
		inline Point GetPositionClientCoords() { return mPositionClientCoords; }
		inline Point GetSizeClientCoords() { return mSizeClientCoords; }
	};
}

#endif // NATIVEUI_CONTROL
