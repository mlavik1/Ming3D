#include "control.h"

namespace NativeUI
{
	Control::Control(Control* arg_parent)
	{
		mParent = arg_parent;
		if (mParent != nullptr)
		{
			mParent->mChildren.push_back(this);
		}
		mSize = Point(25.0f, 25.0f);

		EnableScrollBar(mHwnd, SB_BOTH, ESB_ENABLE_BOTH);
	}

	void Control::UpdateClientCoords()
	{
		float width = mSize.x;
		float height = mSize.y;
		float x = mPosition.x;
		float y = mPosition.y;
	
		const bool relWidth = mHorizontalSizeMode == SizeMode::Relative;
		const bool relHeight = mVerticalSizeMode == SizeMode::Relative;
		const bool relX = mHorizontalPositionMode == SizeMode::Relative;
		const bool relY = mVerticalPositionMode == SizeMode::Relative;

		if (relWidth || relHeight || relX || relY && mParent != nullptr)
		{
			const Point parentSize = mParent->GetSizeClientCoords();
			if (relWidth)
			{
				width = parentSize.x * mSize.x;
			}
			if (relHeight)
			{
				height = parentSize.y * mSize.y;
			}
			if (relX)
			{
				x = parentSize.x * x;
			}
			if (relY)
			{
				y = parentSize.y * y;
			}
		}
		mSizeClientCoords = Point(width, height);
		mPositionClientCoords = Point(x, y);
	}

	void Control::OnTransformUpdated()
	{
		UpdateClientCoords();

		const int x = (int)mPositionClientCoords.x;
		const int y = (int)mPositionClientCoords.y;
		const int w = (int)mSizeClientCoords.x;
		const int h = (int)mSizeClientCoords.y;
		SetWindowPos(mHwnd, HWND_TOP, x, y, w, h, 0);
		OnResize();

		if (mParent != nullptr)
		{
			mParent->OnChildTransformUpdated();
		}
	}

	void Control::OnChildTransformUpdated()
	{

	}

	void Control::SetPosition(const Point& arg_pos)
	{
		mPosition = arg_pos;
		OnTransformUpdated();
	}

	void Control::SetSize(const Point& arg_size)
	{
		mSize = arg_size;
		OnTransformUpdated();
	}

	void Control::SetPosition(const float& arg_x, const float& arg_y)
	{
		SetPosition(Point(arg_x, arg_y));
	}

	void Control::SetSize(const float& arg_w, const float& arg_h)
	{
		SetSize(Point(arg_w, arg_h));
	}

	void Control::SetVerticalSizeMode(const SizeMode& arg_sizemode)
	{
		mVerticalSizeMode = arg_sizemode;
		OnTransformUpdated();
	}

	void Control::SetHorizontalSizeMode(const SizeMode& arg_sizemode)
	{
		mHorizontalSizeMode = arg_sizemode;
		OnTransformUpdated();
	}

	void Control::SetVerticalPositionMode(const SizeMode& arg_sizemode)
	{
		mVerticalPositionMode = arg_sizemode;
		OnTransformUpdated();
	}

	void Control::SetHorizontalPositionMode(const SizeMode& arg_sizemode)
	{
		mHorizontalPositionMode = arg_sizemode;
		OnTransformUpdated();
	}

	void Control::OnUserResize()
	{
		if (mVerticalSizeMode == SizeMode::Absolute || mHorizontalSizeMode == SizeMode::Absolute)
		{
			Point size = GetSize();
			RECT clientRect;
			GetWindowRect(mHwnd, &clientRect);

			if (mHorizontalSizeMode == SizeMode::Absolute)
			{
				size.x = clientRect.right - clientRect.left;
			}
			if (mVerticalSizeMode == SizeMode::Absolute)
			{
				size.y = clientRect.bottom - clientRect.top;
			}

			SetPosition(Point(clientRect.left, clientRect.top));
			SetSize(size);
			OnTransformUpdated(); // Will call OnResize
		}
	}

	void Control::OnResize()
	{
		for (Control* child : mChildren)
		{
			child->OnParentResize();
		}
	}
	
	void Control::OnParentResize()
	{
		OnTransformUpdated(); // Will call OnResize

		if (mVerticalSizeMode == SizeMode::Relative || mHorizontalSizeMode == SizeMode::Relative)
		{
			OnResize(); // relative to parent => needs resize
		}
	}
}
