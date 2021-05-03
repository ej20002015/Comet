#pragma once

#include "Comet/Core/Application.h"
#include "Comet/Core/Log.h"
#include "Comet/Core/Layer.h"
#include "Comet/Core/Input.h"
#include "Comet/Core/PlatformUtilities.h"
#include "Comet/Core/KeyCodes.h"
#include "Comet/Core/MouseCodes.h"
#include "Comet/Core/Timestep.h"

#include "Comet/Events/Event.h"
#include "Comet/Events/ApplicationEvent.h"
#include "Comet/Events/KeyboardEvent.h"
#include "Comet/Events/MouseEvent.h"

#include "Comet/Profiling/ScopeTimer.h"

//Renderer
#include "Comet/Renderer/Renderer.h"
#include "Comet/Renderer/Renderer2D.h"
#include "Comet/Renderer/EditorCamera.h"

//Scene System
#include "Comet/Scene/Scene.h"
#include "Comet/Scene/Entity.h"
#include "Comet/Scene/Components.h"
#include "Comet/Scene/EntityNativeScript.h"
#include "Comet/Scene/SceneSerializer.h"

//maybe temp?
#include "Comet/Renderer/Framebuffer.h"
#include "Comet/Renderer/Camera.h"
#include "Comet/Renderer/OrthographicCamera.h"

//ImGui
#include "Comet/ImGui/ImGuiUtilities.h"