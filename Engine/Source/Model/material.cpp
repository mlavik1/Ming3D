#include "material.h"
#include "GameEngine/game_engine.h"
#include "render_device.h"

namespace Ming3D
{
    Material::Material(ShaderConverter::ParsedShaderProgram* shaderProgram)
    {
        mShaderProgramData = shaderProgram;
        mMaterialBuffer = new MaterialBuffer();

        // TODO: Queue render thread command
        mMaterialBuffer->mShaderProgram = GGameEngine->GetRenderDevice()->CreateShaderProgram(shaderProgram);
    }

    Material::~Material()
    {
        delete mShaderProgramData; // TODO: reference count
    }
}
