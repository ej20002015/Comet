#include "ComponentWidget.h"

#include "glm/gtc/type_ptr.hpp"
#include "glm/gtx/quaternion.hpp"
#include "glm/gtx/matrix_decompose.hpp"

#include <filesystem>

namespace Comet
{

	const float ComponentWidget::s_labelColumnWidth = 160.0f;

	void ComponentWidget::ImGuiRenderUUIDComponentWidget(const UUIDComponent& UUID)
	{
		ImVec2 contentRegion = ImGui::GetContentRegionAvail();
		float contentColumnWidth = contentRegion.x - s_labelColumnWidth;
		ImGui::Columns(2, "UUIDColumns", false);
		ImGui::SetColumnWidth(-1, s_labelColumnWidth);
		ImGui::SetColumnWidth(1, contentColumnWidth);

		ImGuiUtilities::pushFont(ImGuiUtilities::ImGuiFontType::FONT_BOLD);
		ImGui::Text("UUID");
		ImGui::NextColumn();
		ImGuiUtilities::popFont();

		ImGui::PushItemWidth(contentColumnWidth);
		ImGui::Text(static_cast<std::string>(UUID).c_str());
		ImGui::PopItemWidth();

		ImGui::Columns(1);
	}

	void ComponentWidget::ImGuiRenderTagComponentWidget(TagComponent& tagComponent)
	{
		ImVec2 contentRegion = ImGui::GetContentRegionAvail();
		float contentColumnWidth = contentRegion.x - s_labelColumnWidth;
		ImGui::Columns(2, "TagColumns", false);
		ImGui::SetColumnWidth(-1, s_labelColumnWidth);
		ImGui::SetColumnWidth(1, contentColumnWidth);

		ImGuiUtilities::pushFont(ImGuiUtilities::ImGuiFontType::FONT_BOLD);
		ImGui::Text("Tag");
		ImGui::NextColumn();
		ImGuiUtilities::popFont();

		ImGui::PushItemWidth(contentColumnWidth);
		std::string& tagString = tagComponent;
		char textBuffer[256];
		strcpy_s(textBuffer, sizeof(textBuffer), tagString.c_str());
		if (ImGui::InputText("##Tag", textBuffer, sizeof(textBuffer)))
		{
			tagString = std::string(textBuffer);
		}
		ImGui::PopItemWidth();

		ImGui::Columns(1);
	}

	void ComponentWidget::ImGuiRenderTransformComponentWidget(TransformComponent& transformComponent)
	{
		ImVec2 contentRegion = ImGui::GetContentRegionAvail();
		float contentColumnWidth = contentRegion.x - s_labelColumnWidth;
		ImGui::Columns(2, "TransformColumns", false);
		ImGui::SetColumnWidth(-1, s_labelColumnWidth);
		ImGui::SetColumnWidth(1, contentColumnWidth);

		ImGuiUtilities::pushFont(ImGuiUtilities::ImGuiFontType::FONT_BOLD);
		ImGui::Text("Translation");
		ImGui::NextColumn();
		ImGuiUtilities::popFont();

		ImGui::PushItemWidth(contentColumnWidth);
		ImGui::DragFloat3("##Translation", glm::value_ptr(transformComponent.translation), 0.5f);
		ImGui::PopItemWidth();
		ImGui::NextColumn();

		ImGuiUtilities::pushFont(ImGuiUtilities::ImGuiFontType::FONT_BOLD);
		ImGui::Text("Rotation");
		ImGui::NextColumn();
		ImGuiUtilities::popFont();

		ImGui::PushItemWidth(contentColumnWidth);
		glm::vec3 rotationEuler = glm::degrees(glm::eulerAngles(transformComponent.rotation));
		if (ImGui::DragFloat3("##Rotation", glm::value_ptr(rotationEuler), 1.0f))
		{
			//Change euler angles back to quaternions
			glm::quat rotationX = glm::angleAxis(glm::radians(rotationEuler.x), glm::vec3(1.0f, 0.0f, 0.0f));
			glm::quat rotationY = glm::angleAxis(glm::radians(rotationEuler.y), glm::vec3(0.0f, 1.0f, 0.0f));
			glm::quat rotationZ = glm::angleAxis(glm::radians(rotationEuler.z), glm::vec3(0.0f, 0.0f, 1.0f));
			transformComponent.rotation = rotationX * rotationY * rotationZ;
		}
		ImGui::PopItemWidth();
		ImGui::NextColumn();

		ImGuiUtilities::pushFont(ImGuiUtilities::ImGuiFontType::FONT_BOLD);
		ImGui::Text("Scale");
		ImGui::NextColumn();
		ImGuiUtilities::popFont();

		ImGui::PushItemWidth(contentColumnWidth);
		ImGui::DragFloat3("##Scale", glm::value_ptr(transformComponent.scale), 1.0f, 0.001f, 10000.0f);
		ImGui::PopItemWidth();

		ImGui::Columns(1);
	}

	void ComponentWidget::ImGuiRenderCameraComponentWidget(CameraComponent& cameraComponent)
	{
		ImVec2 contentRegion = ImGui::GetContentRegionAvail();
		float contentColumnWidth = contentRegion.x - s_labelColumnWidth;
		ImGui::Columns(2, "CameraColumns", false);
		ImGui::SetColumnWidth(-1, s_labelColumnWidth);
		ImGui::SetColumnWidth(1, contentColumnWidth);

		SceneCamera& sceneCamera = cameraComponent.camera;

		//Primary Camera
		ImGuiUtilities::pushFont(ImGuiUtilities::ImGuiFontType::FONT_BOLD);
		ImGui::Text("Primary Camera");
		ImGui::NextColumn();
		ImGuiUtilities::popFont();

		bool primaryCamera = cameraComponent.primary;
		if (ImGui::Checkbox("##Primary Camera", &primaryCamera))
			cameraComponent.primary = primaryCamera;
		ImGui::NextColumn();

		ImGui::Separator();

		const char* projectionTypes[] = { "Perspective", "Orthographic" };
		const char* currentProjectionType = projectionTypes[static_cast<uint32_t>(sceneCamera.getProjectionType())];

		ImGuiUtilities::pushFont(ImGuiUtilities::ImGuiFontType::FONT_BOLD);
		ImGui::Text("Projection Type");
		ImGui::NextColumn();
		ImGuiUtilities::popFont();

		ImGui::PushItemWidth(contentColumnWidth);
		if (ImGui::BeginCombo("##Projection Type", currentProjectionType, ImGuiComboFlags_NoArrowButton))
		{
			for (uint32_t i = 0; i < 2; i++)
			{
				bool isSelected = projectionTypes[i] == currentProjectionType;
				if (ImGui::Selectable(projectionTypes[i], isSelected))
				{
					currentProjectionType = projectionTypes[i];
					sceneCamera.setProjectionType(static_cast<SceneCamera::ProjectionType>(i));
				}

				// Set the initial focus when opening the combo (scrolling + keyboard navigation focus)
				if (isSelected)
					ImGui::SetItemDefaultFocus();
			}
			ImGui::EndCombo();
		}
		ImGui::PopItemWidth();
		ImGui::NextColumn();

		ImGui::Separator();

		//Only display the relevant fields for the current projection type
		if (sceneCamera.getProjectionType() == SceneCamera::ProjectionType::PERSPECTIVE)
		{
			ImGuiUtilities::pushFont(ImGuiUtilities::ImGuiFontType::FONT_BOLD);
			ImGui::Text("Perspective Verticle FOV");
			ImGui::NextColumn();
			ImGuiUtilities::popFont();

			ImGui::PushItemWidth(contentColumnWidth);
			float perspectiveFOV = sceneCamera.getPerspectiveFOV();
			if (ImGui::DragFloat("##Perspective Verticle FOV", &perspectiveFOV, 2.0f, 0.1f, 100.0f))
				sceneCamera.setPerspectiveFOV(perspectiveFOV);
			ImGui::PopItemWidth();
			ImGui::NextColumn();

			ImGuiUtilities::pushFont(ImGuiUtilities::ImGuiFontType::FONT_BOLD);
			ImGui::Text("Perspective Near Plane");
			ImGui::NextColumn();
			ImGuiUtilities::popFont();

			ImGui::PushItemWidth(contentColumnWidth);
			float perspectiveNearPlane = sceneCamera.getPerspectiveNearPlane();
			if (ImGui::DragFloat("##Perspective Near Plane", &perspectiveNearPlane, 0.01f, 0.001f, 1000.0f))
				sceneCamera.setPerspectiveNearPlane(perspectiveNearPlane);
			ImGui::PopItemWidth();
			ImGui::NextColumn();

			ImGuiUtilities::pushFont(ImGuiUtilities::ImGuiFontType::FONT_BOLD);
			ImGui::Text("Perspective Far Plane");
			ImGui::NextColumn();
			ImGuiUtilities::popFont();

			ImGui::PushItemWidth(contentColumnWidth);
			float perspectiveFarPlane = sceneCamera.getPerspectiveFarPlane();
			if (ImGui::DragFloat("Perspective Far Plane", &perspectiveFarPlane, 50.0f, 0.001f, 10000.0f))
				sceneCamera.setPerspectiveFarPlane(perspectiveFarPlane);
			ImGui::PopItemWidth();
			ImGui::NextColumn();
		}
		else
		{
			ImGuiUtilities::pushFont(ImGuiUtilities::ImGuiFontType::FONT_BOLD);
			ImGui::Text("Orthographic Size");
			ImGui::NextColumn();
			ImGuiUtilities::popFont();

			ImGui::PushItemWidth(contentColumnWidth);
			float orthographicSize = sceneCamera.getOrthographicSize();
			if (ImGui::DragFloat("##Orthographic Size", &orthographicSize, 5.0f, 0.1f, 1000.0f))
				sceneCamera.setOrthographicSize(orthographicSize);
			ImGui::PopItemWidth();
			ImGui::NextColumn();

			ImGuiUtilities::pushFont(ImGuiUtilities::ImGuiFontType::FONT_BOLD);
			ImGui::Text("Orthographic Near Plane");
			ImGui::NextColumn();
			ImGuiUtilities::popFont();

			ImGui::PushItemWidth(contentColumnWidth);
			float orthographicNearPlane = sceneCamera.getOrthographicNearPlane();
			if (ImGui::DragFloat("##Orthographic Near Plane", &orthographicNearPlane, 1.0f, -100.0f, 100.0f))
				sceneCamera.setOrthographicNearPlane(orthographicNearPlane);
			ImGui::PopItemWidth();
			ImGui::NextColumn();

			ImGuiUtilities::pushFont(ImGuiUtilities::ImGuiFontType::FONT_BOLD);
			ImGui::Text("Orthographic Far Plane");
			ImGui::NextColumn();
			ImGuiUtilities::popFont();

			ImGui::PushItemWidth(contentColumnWidth);
			float orthographicFarPlane = sceneCamera.getOrthographicFarPlane();
			if (ImGui::DragFloat("##Orthographic Far Plane", &orthographicFarPlane, 1.0f, -100.0f, 100.0f))
				sceneCamera.setOrthographicFarPlane(orthographicFarPlane);
			ImGui::PopItemWidth();
			ImGui::NextColumn();
		}

		ImGui::Separator();

		ImGuiUtilities::pushFont(ImGuiUtilities::ImGuiFontType::FONT_BOLD);
		ImGui::Text("Fixed Aspect Ratio");
		ImGui::NextColumn();
		ImGuiUtilities::popFont();

		ImGui::PushItemWidth(contentColumnWidth);
		bool fixedAspectRatio = sceneCamera.getFixedAspectRatio();
		if (ImGui::Checkbox("##Fixed Aspect Ratio", &fixedAspectRatio))
			sceneCamera.setFixedAspectRatio(fixedAspectRatio);
		ImGui::PopItemWidth();
		ImGui::NextColumn();

		//Only allow the user to manually adjust the aspect ratio if the camera has a fixed aspect ratio
		if (fixedAspectRatio)
		{
			ImGuiUtilities::pushFont(ImGuiUtilities::ImGuiFontType::FONT_BOLD);
			ImGui::Text("Aspect Ratio");
			ImGui::NextColumn();
			ImGuiUtilities::popFont();

			ImGui::PushItemWidth(contentColumnWidth);
			float aspectRatio = sceneCamera.getAspectRatio();
			if (ImGui::DragFloat("##Aspect Ratio", &aspectRatio, 0.1f, 0.1f, 10.0f))
				sceneCamera.setAspectRatio(aspectRatio);
			ImGui::PopItemWidth();
		}

		ImGui::Columns(1);
	}

	void ComponentWidget::ImGuiRenderSpriteComponentWidget(SpriteComponent& spriteComponent)
	{
		ImVec2 contentRegion = ImGui::GetContentRegionAvail();
		float contentColumnWidth = contentRegion.x - s_labelColumnWidth;
		ImGui::Columns(2, "SpriteColumns", false);
		ImGui::SetColumnWidth(-1, s_labelColumnWidth);
		ImGui::SetColumnWidth(1, contentColumnWidth);

		ImGuiUtilities::pushFont(ImGuiUtilities::ImGuiFontType::FONT_BOLD);
		ImGui::Text("Color");
		ImGui::NextColumn();
		ImGuiUtilities::popFont();

		ImGui::PushItemWidth(contentColumnWidth);
		glm::vec4 color = spriteComponent.color;
		if (ImGui::ColorEdit4("##Color", glm::value_ptr(color)))
			spriteComponent.color = color;
		ImGui::PopItemWidth();
		ImGui::NextColumn();

		ImGui::Separator();

		ImGuiUtilities::pushFont(ImGuiUtilities::ImGuiFontType::FONT_BOLD);
		ImGui::Text("Texture Filepath");
		ImGui::NextColumn();
		ImGuiUtilities::popFont();

		ImGui::PushItemWidth(contentColumnWidth);
		std::string textureFilepath = (spriteComponent.texture) ? spriteComponent.texture->getFilepath() : "";
		char textBuffer[512];
		strcpy_s(textBuffer, sizeof(textBuffer), textureFilepath.c_str());
		if (ImGui::InputText("##Texture Filepath", textBuffer, sizeof(textBuffer)))
		{
			textureFilepath = std::string(textBuffer);
			if (!textureFilepath.size())
			{
				spriteComponent.texture = nullptr;
				spriteComponent.subTexture.setTextureAtlas(nullptr);
			}
			else
			{
				//TODO: Needs cleaning up
				std::filesystem::path textureFilepathPath = textureFilepath;
				auto extension = textureFilepathPath.extension();
				if (!extension.empty())
				{
					if (extension == ".png" || extension == ".jpg")
					{
						if (std::filesystem::exists(textureFilepath))
						{
							auto texture = Texture2D::create(textureFilepath);
							spriteComponent.texture = texture;
							spriteComponent.subTexture.setTextureAtlas(texture);
						}
					}
				}
			}
		}
		ImGui::PopItemWidth();
		ImGui::NextColumn();

		//Only show texture options if texture is set
		if (spriteComponent.texture)
		{
			ImGuiUtilities::pushFont(ImGuiUtilities::ImGuiFontType::FONT_BOLD);
			ImGui::Text("Tiling Factor");
			ImGui::NextColumn();
			ImGuiUtilities::popFont();

			ImGui::PushItemWidth(contentColumnWidth);
			float tilingFactor = spriteComponent.tilingFactor;
			if (ImGui::DragFloat("##Tiling Factor", &tilingFactor, 1.0f, 1.0f, 1000.0f))
				spriteComponent.tilingFactor = tilingFactor;
			ImGui::PopItemWidth();
			ImGui::NextColumn();

			ImGuiUtilities::pushFont(ImGuiUtilities::ImGuiFontType::FONT_BOLD);
			ImGui::Text("Sprite Texture Type");
			ImGui::NextColumn();
			ImGuiUtilities::popFont();

			const char* textureTypes[] = { "Normal", "Sub-texture" };
			const char* currentTextureType = textureTypes[static_cast<uint32_t>(spriteComponent.spriteTextureType)];

			ImGui::PushItemWidth(contentColumnWidth);
			if (ImGui::BeginCombo("##Sprite Texture Type", currentTextureType, ImGuiComboFlags_NoArrowButton))
			{
				for (uint32_t i = 0; i < 2; i++)
				{
					bool isSelected = textureTypes[i] == currentTextureType;
					if (ImGui::Selectable(textureTypes[i], isSelected))
					{
						currentTextureType = textureTypes[i];
						spriteComponent.spriteTextureType = static_cast<SpriteComponent::SpriteTextureType>(i);
					}

					// Set the initial focus when opening the combo (scrolling + keyboard navigation focus)
					if (isSelected)
						ImGui::SetItemDefaultFocus();
				}
				ImGui::EndCombo();
			}
			ImGui::PopItemWidth();
			ImGui::NextColumn();

			if (spriteComponent.spriteTextureType == SpriteComponent::SpriteTextureType::SUB_TEXTURE)
			{
				ImGui::Separator();

				Texture2DSubTexture& subTexture = spriteComponent.subTexture;

				ImGuiUtilities::pushFont(ImGuiUtilities::ImGuiFontType::FONT_BOLD);
				ImGui::Text("Cell Size");
				ImGui::NextColumn();
				ImGuiUtilities::popFont();

				ImGui::PushItemWidth(contentColumnWidth);
				int32_t cellSize = static_cast<int32_t>(subTexture.getCellSize());
				if (ImGui::DragInt("##Cell Size", &cellSize, 1.0f, 0, 1024))
					subTexture.setCellSize(static_cast<uint32_t>(cellSize));
				ImGui::PopItemWidth();
				ImGui::NextColumn();

				ImGuiUtilities::pushFont(ImGuiUtilities::ImGuiFontType::FONT_BOLD);
				ImGui::Text("Texture Atlas Index");
				ImGui::NextColumn();
				ImGuiUtilities::popFont();

				ImGui::PushItemWidth(contentColumnWidth);
				glm::vec2 textureAtlasIndex = subTexture.getTextureAtlasIndex();
				if (ImGui::DragFloat2("##Texture Atlas Index", glm::value_ptr(textureAtlasIndex), 1.0f, 0.0f, 1024))
					subTexture.setTextureAtlasIndex(textureAtlasIndex);
				ImGui::PopItemWidth();
				ImGui::NextColumn();

				ImGuiUtilities::pushFont(ImGuiUtilities::ImGuiFontType::FONT_BOLD);
				ImGui::Text("Texture Scale");
				ImGui::NextColumn();
				ImGuiUtilities::popFont();

				ImGui::PushItemWidth(contentColumnWidth);
				glm::vec2 textureScale = subTexture.getTextureScale();
				if (ImGui::DragFloat2("##Texture Scale", glm::value_ptr(textureScale), 0.2f, 0.001f, 1024))
					subTexture.setTextureScale(textureScale);
				ImGui::PopItemWidth();
			}
		}

		ImGui::Columns(1);

	}

}
