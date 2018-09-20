#ifndef MING3D_EDITOR_H
#define MING3D_EDITOR_H

#include "editor_window.h"

namespace Ming3D
{
	class Editor
	{
	private:
		Editor();
		EditorWindow* mEditorWinndow = nullptr;

	public:
		static Editor* CreateEditor();
		
		void RunEditor();
	};

	extern Editor* GEditor;
}

#endif
