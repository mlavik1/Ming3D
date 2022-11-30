#ifndef MING3D_SCENE_VIEW_H
#define MING3D_SCENE_VIEW_H

#include "user_control.h"

#include "window_base.h"
#include "render_device.h"
#include "render_window.h"

namespace Ming3D
{
	class SceneView : public NativeUI::UserControl
	{
    private:
        WindowBase* mMainWindow;
        RenderDevice* mRenderDevice;
        RenderWindow* mRenderWindow;
        RenderTarget* mRenderTarget;

	public:
		SceneView(Control* arg_parent);

		virtual void OnPaint() override;
        virtual bool HasFocus();

        void OnTick();
	};
}

#endif
