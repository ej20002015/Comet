#pragma once
#include "CometPCH.h"

#include "Comet/Scene/EntityNativeScript.h"

namespace Comet
{

class CameraControllerScript : public EntityNativeScript
{
public:
    DEF_SCRIPT(CameraControllerScript)

protected:
    void onUpdate(Timestep ts) override;

private:
    const float m_cameraSpeed = 4.0f;
};

}