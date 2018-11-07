
#include <Windows.h>
#include <gl/glew.h>

#include "editor.h"

#pragma comment(linker, "/manifestdependency:\"type='win32' name='Microsoft.Windows.Common-Controls' version='6.0.0.0' processorArchitecture='*' publicKeyToken='6595b64144ccf1df' language='*'\"")  

using namespace Ming3D;

int main()
{
    Editor* editor = editor->CreateEditor();
    editor->RunEditor();

    return 0;
}
