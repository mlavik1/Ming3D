
#include <Windows.h>
#include <gl/glew.h>

#include "editor.h"

using namespace Ming3D;

int main()
{
    Editor* editor = editor->CreateEditor();
    editor->RunEditor();

    return 0;
}
