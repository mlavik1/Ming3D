#if MING3D_TESTTYPE == 10
#include <iostream>

#include "CoreTest.h"
#include "Object/object.h"

#include "Object/function.h"
#include "GameEngine/game_engine.h"
#include "World/world.h"
#include "Actors/actor.h"
#include "Components/mesh_component.h"
#include "Model/model_helper.h"
#include "Components/camera_component.h"
#include "glm/gtx//rotate_vector.hpp"
#include "Input/input_manager.h"
#include "Model/primitive_factory.h"
#include "Model/material_factory.h"
#include "Texture/texture.h"
#include "Texture/texture_loader.h"
#include "GUI/image_widget.h"
#include "Components/widget_component.h"
#include "GUI/text_widget.h"
#include "3rdparty/tinyxml2/tinyxml2.h"
#include <vector>
#include <string>

using namespace Ming3D;

struct WidgetSizeValue
{
    WidgetSizeMode mWidgetSizeMode = WidgetSizeMode::Absolute;
    float mValue = 0.0f;
};

std::vector<std::string> splitString(const std::string text, const char delimiter)
{
    std::vector<std::string> vec;
    size_t iStart = 0;
    size_t iCurr = 1;
    for(int iCurr = 0; iCurr < text.size(); iCurr++)
    {
        if (text[iCurr] == ' ')
        {
            std::string substr = text.substr(iStart, iCurr - iStart);
            vec.push_back(substr);
            iStart = iCurr + 1;
        }
    }
    if (iStart < text.size())
    {
        std::string substr = text.substr(iStart, text.size() - iStart);
        vec.push_back(substr);
    }
    return vec;
}

glm::vec4 parseVec4(const std::string text)
{
    glm::vec4 vec;
    std::vector<std::string> split = splitString(text, ' ');
    
    for (int i = 0; i < 4 && i < split.size(); i++)
    {
        vec[i] = std::stof(split[i]);
    }
    return vec;
}

WidgetSizeValue parseWidgetSizeValue(const std::string text)
{
    WidgetSizeValue sizeVal;
    if (text.size() > 1 && text[text.size() - 1] == '%')
    {
        sizeVal.mWidgetSizeMode = WidgetSizeMode::Relative;
        sizeVal.mValue = std::stof(text.substr(0, text.size() - 1)) / 100.0f;
    }
    else if (text.size() > 2 && text.substr(text.size() - 2, 2) == "px")
    {
        sizeVal.mWidgetSizeMode = WidgetSizeMode::Absolute;
        sizeVal.mValue = std::stof(text.substr(0, text.size() - 2));
    }
    else
    {
        sizeVal.mWidgetSizeMode = WidgetSizeMode::Absolute;
        sizeVal.mValue = std::stof(text);
    }
    return sizeVal;
}

Widget* parseWidgetNode(tinyxml2::XMLElement* node)
{
    Widget* widget = nullptr;

    std::string nodeVal = node->Value();
    std::cout << nodeVal << std::endl;

    if (nodeVal == "CustomWidget")
    {
        widget = new Widget();
    }
    else if (nodeVal == "ImageWidget")
    {
        ImageWidget* imgWidget = new ImageWidget();
        const tinyxml2::XMLAttribute* colAttr = node->FindAttribute("colour");
        if (colAttr != nullptr)
            imgWidget->SetColour(parseVec4(colAttr->Value()) / 255.0f);
        widget = imgWidget;
    }

    WidgetSizeValue xVal, yVal, wVal, hVal;
    const tinyxml2::XMLAttribute* xAttr = node->FindAttribute("x");
    const tinyxml2::XMLAttribute* yAttr = node->FindAttribute("y");
    const tinyxml2::XMLAttribute* wAttr = node->FindAttribute("width");
    const tinyxml2::XMLAttribute* hAttr = node->FindAttribute("height");
    if (xAttr != nullptr)
        xVal = parseWidgetSizeValue(xAttr->Value());
    if (yAttr != nullptr)
        yVal = parseWidgetSizeValue(yAttr->Value());
    if (wAttr != nullptr)
        wVal = parseWidgetSizeValue(wAttr->Value());
    if (hAttr != nullptr)
        hVal = parseWidgetSizeValue(hAttr->Value());

    widget->setHorizontalPositioning(xVal.mWidgetSizeMode);
    widget->setVerticalPositioning(yVal.mWidgetSizeMode);
    widget->setHorizontalScaling(wVal.mWidgetSizeMode);
    widget->setVerticalScaling(hVal.mWidgetSizeMode);
    widget->setPosition(xVal.mValue, yVal.mValue);
    widget->setSize(wVal.mValue, hVal.mValue);

    tinyxml2::XMLElement* currChild = node->FirstChildElement();
    while (currChild != nullptr)
    {
        Widget* childWidget = parseWidgetNode(currChild);
        if (childWidget != nullptr)
            widget->addWidget(childWidget);
        currChild = currChild->NextSiblingElement();
    }
    return widget;
}

Widget* loadWidgetFromXML(const std::string widgetPath)
{
    tinyxml2::XMLDocument doc;
    doc.LoadFile(widgetPath.c_str());

    tinyxml2::XMLElement* root = doc.RootElement();
    return parseWidgetNode(root);
}

int main()
{
    GameEngine* gameEngine = new GameEngine();
    gameEngine->Initialise();

    Actor* camActor = new Actor();
    camActor->AddComponent<CameraComponent>();
    camActor->GetTransform().SetWorldPosition(glm::vec3(0.0f, 2.0f, 6.0f));
    gameEngine->GetWorld()->AddActor(camActor);

    Actor* skybox = new Actor();
    skybox->GetTransform().SetLocalPosition(glm::vec3(1.5f, 0.0f, 0.0f));
    skybox->GetTransform().SetLocalScale(glm::vec3(-50.0f, 50.0f, 50.0f));
    gameEngine->GetWorld()->AddActor(skybox);
    ModelLoader::LoadModel(GGameEngine->GetResourceDirectory() + std::string("/Skybox/Skybox.obj"), skybox, MODELLOADERFLAGS_UNLIT | MODELLOADERFLAGS_FORCE_OPAQUE);

    Actor* actor1 = new Actor();
    actor1->GetTransform().SetLocalPosition(glm::vec3(1.5f, 0.0f, 0.0f));
    actor1->GetTransform().SetLocalScale(glm::vec3(50.0f, 50.0f, 50.0f));
    actor1->GetTransform().SetLocalRotation(glm::angleAxis(10.0f * 3.141592654f / 180.0f, glm::vec3(0.0f, 1.0f, 0.0f)));
    gameEngine->GetWorld()->AddActor(actor1);
    ModelLoader::LoadModel(GGameEngine->GetResourceDirectory() + std::string("/Mvr_PetCow_walk.dae"), actor1);





    const std::string resourceDir = std::string(MING3D_SAMPLES_SOURCE_DIR) + std::string("/Resources");
    const std::string testGuiPath = resourceDir + std::string("/gui_test.widget");



    Widget* testtest = loadWidgetFromXML(testGuiPath);



    Actor* guiActor = new Actor();
    guiActor->GetTransform().SetLocalPosition(glm::vec3(0.0f, 0.0f, 0.0f));
    guiActor->GetTransform().SetLocalScale(glm::vec3(0.005f, 0.005f, 0.005f));
    WidgetComponent* widgetComp = guiActor->AddComponent<WidgetComponent>();
    Widget* rootWidget = testtest;
    /*Widget* rootWidget = new Widget();
    rootWidget->setPosition(0.0f, 0.0f, WidgetSizeMode::Absolute);
    rootWidget->setSize(800.0f, 600.0f, WidgetSizeMode::Absolute);
    ImageWidget* img1 = new ImageWidget();
    img1->setPosition(0.0f, 0.0f, WidgetSizeMode::Relative);
    img1->setSize(0.5f, 0.5f, WidgetSizeMode::Relative);
    img1->SetColour(glm::vec4(1.0f, 0.0f, 0.0f, 1.0f));
    rootWidget->addWidget(img1);



    TextWidget* txtWidget = new TextWidget();
    txtWidget->setPosition(0.5f, 0.5f, WidgetSizeMode::Relative);
    txtWidget->setSize(0.5f, 0.5f, WidgetSizeMode::Relative);
    txtWidget->SetFont("D:/FreeSans.ttf", 42);
    txtWidget->SetText(L"Hello Ming3D!");
    txtWidget->SetColour(glm::vec4(1.0f, 1.0f, 1.0f, 1.0f));
    rootWidget->addWidget(txtWidget);*/



    widgetComp->SetWidget(rootWidget);
    gameEngine->GetWorld()->AddActor(guiActor);

    const float camSpeed = 3.0f;
    const float camRotSpeed = 1.0f;

    while (true)
    {
        gameEngine->Update();
        actor1->GetTransform().Rotate(0.001f, glm::vec3(0, 1, 0));

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

#endif
