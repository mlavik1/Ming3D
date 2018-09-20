#ifndef MING3D_EDITORWINDOW_H
#define MING3D_EDITORWINDOW_H

#include "window.h"
#include "scene_view.h"

namespace Ming3D
{
	class EditorWindow : public NativeUI::Window
	{
	private:
		SceneView* mSceneView;

	public:
		EditorWindow();

		inline SceneView* GetSceneView() { return mSceneView; }

        void OnTick();
	};
}

#endif
