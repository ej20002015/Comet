#pragma once

#include <Comet.h>

#include "imgui.h"

namespace Comet
{

	class ComponentWidget
	{
	public:
		ComponentWidget() = delete;

		static void ImGuiRenderUUIDComponentWidget(const UUIDComponent& UUID);
		static void ImGuiRenderTagComponentWidget(TagComponent& tagComponent);
		static void ImGuiRenderTransformComponentWidget(TransformComponent& transformComponent);
		static void ImGuiRenderCameraComponentWidget(CameraComponent& cameraComponent);
		static void ImGuiRenderSpriteComponentWidget(SpriteComponent& spriteComponent);

		static const float s_labelColumnWidth;
	};

}