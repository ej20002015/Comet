#include "ComponentWidget.h"

#include "glm/gtc/type_ptr.hpp"
#include "glm/gtx/quaternion.hpp"
#include "glm/gtx/matrix_decompose.hpp"

#include <filesystem>

namespace Comet
{

	void ComponentWidget::ImGuiRenderUUIDComponentWidget(const UUIDComponent& UUID)
	{
		ImGui::Text("UUID: %s", static_cast<std::string>(UUID).c_str());
	}

	void ComponentWidget::ImGuiRenderTagComponentWidget(TagComponent& tagComponent)
	{
		std::string& tagString = tagComponent;
		char textBuffer[256];
		strcpy_s(textBuffer, sizeof(textBuffer), tagString.c_str());
		if (ImGui::InputText("Tag", textBuffer, sizeof(textBuffer)))
		{
			tagString = std::string(textBuffer);
		}
	}

	void ComponentWidget::ImGuiRenderTransformComponentWidget(TransformComponent& transformComponent)
	{
		glm::vec3 rotationEuler = glm::degrees(glm::eulerAngles(transformComponent.rotation));

		ImGui::DragFloat3("Translation", glm::value_ptr(transformComponent.translation), 0.5f);

		if (ImGui::SliderFloat3("Rotation", glm::value_ptr(rotationEuler), 0.0f, 360.0f))
		{
			//Change euler angles back to quaternions
			glm::quat rotationX = glm::angleAxis(glm::radians(rotationEuler.x), glm::vec3(1.0f, 0.0f, 0.0f));
			glm::quat rotationY = glm::angleAxis(glm::radians(rotationEuler.y), glm::vec3(0.0f, 1.0f, 0.0f));
			glm::quat rotationZ = glm::angleAxis(glm::radians(rotationEuler.z), glm::vec3(0.0f, 0.0f, 1.0f));
			transformComponent.rotation = rotationX * rotationY * rotationZ;
		}

		ImGui::DragFloat3("Scale", glm::value_ptr(transformComponent.scale), 1.0f, 0.001f, 10000.0f);
	}

	void ComponentWidget::ImGuiRenderCameraComponentWidget(CameraComponent& cameraComponent)
	{
		SceneCamera& sceneCamera = cameraComponent.camera;

		const char* projectionTypes[] = { "Perspective", "Orthographic" };
		const char* currentProjectionType = projectionTypes[static_cast<uint32_t>(sceneCamera.getProjectionType())];

		//Primary Camera
		bool primaryCamera = cameraComponent.primary;
		if (ImGui::Checkbox("Primary Camera", &primaryCamera))
			cameraComponent.primary = primaryCamera;

		ImGui::Separator();

		if (ImGui::BeginCombo("Projection Type", currentProjectionType))
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

		ImGui::Separator();

		//Only display the relevant fields for the current projection type
		if (sceneCamera.getProjectionType() == SceneCamera::ProjectionType::PERSPECTIVE)
		{
			float perspectiveFOV = sceneCamera.getPerspectiveFOV();
			if (ImGui::DragFloat("Perspective Verticle FOV", &perspectiveFOV, 2.0f, 0.1f, 100.0f))
				sceneCamera.setPerspectiveFOV(perspectiveFOV);

			float perspectiveNearPlane = sceneCamera.getPerspectiveNearPlane();
			if (ImGui::DragFloat("Perspective Near Plane", &perspectiveNearPlane, 0.01f, 0.001f, 1000.0f))
				sceneCamera.setPerspectiveNearPlane(perspectiveNearPlane);

			float perspectiveFarPlane = sceneCamera.getPerspectiveFarPlane();
			if (ImGui::DragFloat("Perspective Far Plane", &perspectiveFarPlane, 50.0f, 0.001f, 10000.0f))
				sceneCamera.setPerspectiveFarPlane(perspectiveFarPlane);
		}
		else
		{
			float orthographicSize = sceneCamera.getOrthographicSize();
			if (ImGui::DragFloat("Orthographic Size", &orthographicSize, 5.0f, 0.1f, 1000.0f))
				sceneCamera.setOrthographicSize(orthographicSize);

			float orthographicNearPlane = sceneCamera.getOrthographicNearPlane();
			if (ImGui::DragFloat("Orthographic Near Plane", &orthographicNearPlane, 1.0f, -100.0f, 100.0f))
				sceneCamera.setOrthographicNearPlane(orthographicNearPlane);

			float orthographicFarPlane = sceneCamera.getOrthographicFarPlane();
			if (ImGui::DragFloat("Orthographic Far Plane", &orthographicFarPlane, 1.0f, -100.0f, 100.0f))
				sceneCamera.setOrthographicFarPlane(orthographicFarPlane);
		}

		ImGui::Separator();

		bool fixedAspectRatio = sceneCamera.getFixedAspectRatio();
		if (ImGui::Checkbox("Fixed Aspect Ratio", &fixedAspectRatio))
			sceneCamera.setFixedAspectRatio(fixedAspectRatio);

		//Only allow the user to manually adjust the aspect ratio if the camera has a fixed aspect ratio
		if (fixedAspectRatio)
		{
			float aspectRatio = sceneCamera.getAspectRatio();
			if (ImGui::DragFloat("Aspect Ratio", &aspectRatio, 0.1f, 0.1f, 10.0f))
				sceneCamera.setAspectRatio(aspectRatio);
		}
	}

	void ComponentWidget::ImGuiRenderSpriteComponentWidget(SpriteComponent& spriteComponent)
	{
		glm::vec4 color = spriteComponent.color;
		if (ImGui::ColorEdit4("Color", glm::value_ptr(color)))
			spriteComponent.color = color;

		std::string textureFilepath = (spriteComponent.texture) ? spriteComponent.texture->getFilepath() : "";
		char textBuffer[512];
		strcpy_s(textBuffer, sizeof(textBuffer), textureFilepath.c_str());
		if (ImGui::InputText("Texture Filepath", textBuffer, sizeof(textBuffer)))
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

		//Only show texture options if texture is set
		if (spriteComponent.texture)
		{
			float tilingFactor = spriteComponent.tilingFactor;
			if (ImGui::DragFloat("Tiling Factor", &tilingFactor, 1.0f, 1.0f, 1000.0f))
				spriteComponent.tilingFactor = tilingFactor;

			const char* textureTypes[] = { "Normal", "Sub-texture" };
			const char* currentTextureType = textureTypes[static_cast<uint32_t>(spriteComponent.spriteTextureType)];

			if (ImGui::BeginCombo("Sprite Texture Type", currentTextureType))
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

			if (spriteComponent.spriteTextureType == SpriteComponent::SpriteTextureType::SUB_TEXTURE)
			{
				Texture2DSubTexture& subTexture = spriteComponent.subTexture;

				int32_t cellSize = static_cast<int32_t>(subTexture.getCellSize());
				if (ImGui::DragInt("Cell Size", &cellSize, 1.0f, 0, 1024))
					subTexture.setCellSize(static_cast<uint32_t>(cellSize));

				glm::vec2 textureAtlasIndex = subTexture.getTextureAtlasIndex();
				if (ImGui::DragFloat2("Texture Atlas Index", glm::value_ptr(textureAtlasIndex), 1.0f, 0.0f, 1024))
					subTexture.setTextureAtlasIndex(textureAtlasIndex);

				glm::vec2 textureScale = subTexture.getTextureScale();
				if (ImGui::DragFloat2("texture Scale", glm::value_ptr(textureScale), 0.2f, 0.001f, 1024))
					subTexture.setTextureScale(textureScale);
			}
		}

	}

}
