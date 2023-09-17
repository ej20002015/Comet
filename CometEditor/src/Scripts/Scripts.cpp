#include "CometPCH.h"
#include "Scripts.h"

#include "Comet/Scene/Components.h"
#include "Comet/Core/Input.h"
#include "Comet/Scene/ScriptRegistry.h"

namespace Comet
{

void CameraControllerScript::onUpdate(Timestep ts)
{
    TransformComponent& transformComponent = getComponent<TransformComponent>();

    if (Input::isKeyPressed(KeyCode::KEY_D))
        transformComponent.translation.x += m_cameraSpeed * ts;
    else if (Input::isKeyPressed(KeyCode::KEY_A))
        transformComponent.translation.x -= m_cameraSpeed * ts;

    if (Input::isKeyPressed(KeyCode::KEY_W))
        transformComponent.translation.y += m_cameraSpeed * ts;
    else if (Input::isKeyPressed(KeyCode::KEY_S))
        transformComponent.translation.y -= m_cameraSpeed * ts;
}

REG_SCRIPT(CameraControllerScript)

}