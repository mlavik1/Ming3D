#include <iostream>

#include "GameEngine/game_engine.h"

#include "World/world.h"
#include "Actors/actor.h"
#include "Components/mesh_component.h"
#include "Model/model_helper.h"
#include "Components/camera_component.h"
#include "glm/gtx//rotate_vector.hpp"
#include "Input/input_manager.h"
#include "Platform/platform.h"
#include "Components/mesh_component.h"
#include "Debug/debug.h"
#include <sstream>
#include <vector>
#include <algorithm>
#include <limits>

using namespace Ming3D;

void LoadModel(Actor* actor)
{
    std::string filePath;
    if (GGameEngine->GetPlatform()->mPlatformFile->OpenFileDialog("Select model to load", filePath))
    {
        ModelLoader::LoadModel(filePath.c_str(), actor);
    }
    else
    {
        std::string modelPath = GGameEngine->GetPlatform()->ReadConsoleLine();
        std::stringstream pathstream;
        for (const char c : modelPath)
        {
            if (c != '\'' && c != '"')
                pathstream << c;
        }
        modelPath = pathstream.str();
        ModelLoader::LoadModel(modelPath.c_str(), actor);
    }
}

int main()
{
	GameEngine* gameEngine = new GameEngine();
	gameEngine->Initialise();

    auto world = gameEngine->GetWorld().lock();
    
    Actor* camActor = world->SpawnActor();
    camActor->AddComponent<CameraComponent>();
    camActor->GetTransform().SetWorldPosition(glm::vec3(0.0f, 0.0f, 3.0f));

    Actor* modelActor = world->SpawnActor();
    modelActor->GetTransform().SetLocalPosition(glm::vec3(0.0f, 0.0f, 0.0f));
    modelActor->GetTransform().SetLocalScale(glm::vec3(1.0f, 1.0f, 1.0f));
    modelActor->GetTransform().SetLocalRotation(glm::angleAxis(10.0f * 3.141592654f / 180.0f, glm::vec3(0.0f, 1.0f, 0.0f)));
    LoadModel(modelActor);

    // Normalise mesh scale
    glm::vec3 minPos(std::numeric_limits<float>::max(), std::numeric_limits<float>::max(), std::numeric_limits<float>::max());
    glm::vec3 maxPos(std::numeric_limits<float>::lowest(), std::numeric_limits<float>::lowest(), std::numeric_limits<float>::lowest());
    std::vector<MeshComponent*> meshComps = modelActor->GetComponentsInChildren<MeshComponent>();
    for(MeshComponent* meshComp : meshComps)
    {
        // TODO: Create vertex iterator class
        std::shared_ptr<Mesh> mesh = meshComp->GetMesh();
        Rendering::VertexData* vertData = mesh->mVertexData;
        Rendering::VertexDataIterator<glm::vec3> vertIterator(vertData, Rendering::EVertexComponent::Position);
        size_t vertCount = vertIterator.GetCount();
        for (size_t iVert = 0; iVert < vertCount; iVert++)
        {
            const glm::vec3 pos = vertIterator.GetElement(iVert);
            minPos.x = std::min(minPos.x, pos.x);
            minPos.y = std::min(minPos.y, pos.y);
            minPos.z = std::min(minPos.z, pos.z);
            maxPos.x = std::max(maxPos.x, pos.x);
            maxPos.y = std::max(maxPos.y, pos.y);
            maxPos.z = std::max(maxPos.z, pos.z);
        }
    }
    glm::vec3 bounds = maxPos - minPos;
    float greatestAxis = std::max(std::max(bounds.x, bounds.y), bounds.z);
    modelActor->GetTransform().SetLocalScale(glm::vec3(1.0f, 1.0f, 1.0f) / greatestAxis);
    //glm::vec3 meshCentre = ((maxPos + minPos) * 0.5f) / greatestAxis;
    //modelActor->GetTransform().SetLocalPosition(-meshCentre);
    
    const float camSpeed = 3.0f;
    const float camRotSpeed = 1.0f;

    while (true)
    {
        gameEngine->Update();
        modelActor->GetTransform().Rotate(0.001f, glm::vec3(0,1,0));

        float speed = camSpeed * GGameEngine->GetDeltaTime();
        float rotSpeed = camRotSpeed * GGameEngine->GetDeltaTime();
        Transform& camTrans = camActor->GetTransform();

        glm::vec3 camPos = camTrans.GetWorldPosition();
        if (GGameEngine->GetInputManager()->GetKey(KeyCode::Key_W))
            camPos += camTrans.GetUp() * speed;
        else if (GGameEngine->GetInputManager()->GetKey(KeyCode::Key_S))
            camPos += camTrans.GetUp() * -speed;
        else if (GGameEngine->GetInputManager()->GetKey(KeyCode::Key_D))
            camPos += camTrans.GetRight() * speed;
        else if (GGameEngine->GetInputManager()->GetKey(KeyCode::Key_A))
            camPos += camTrans.GetRight() * -speed;
        else if (GGameEngine->GetInputManager()->GetKey(KeyCode::Key_Up))
            camPos += camTrans.GetForward() * speed;
        else if (GGameEngine->GetInputManager()->GetKey(KeyCode::Key_Down))
            camPos += camTrans.GetForward() * -speed;

        camTrans.SetWorldPosition(camPos);

        if (GGameEngine->GetInputManager()->GetKey(KeyCode::Key_Right))
            camTrans.Rotate(-rotSpeed, glm::vec3(0.0f, 1.0f, 0.0f));
        else if (GGameEngine->GetInputManager()->GetKey(KeyCode::Key_Left))
            camTrans.Rotate(rotSpeed, glm::vec3(0.0f, 1.0f, 0.0f));
    }
	return 0;
}
