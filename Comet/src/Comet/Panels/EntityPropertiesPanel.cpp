#include "CometPCH.h"
#include "EntityPropertiesPanel.h"

#include "Comet/Core/PlatformUtilities.h"
#include "Comet/ImGui/ImGuiUtilities.h"

#include "glm/gtc/type_ptr.hpp"
#include "glm/gtx/quaternion.hpp"
#include "glm/gtx/matrix_decompose.hpp"

#include "imgui.h"
#include "imgui_internal.h"

namespace Comet
{

	const float EntityPropertiesPanel::s_labelColumnWidth = 160.0f;

	EntityPropertiesPanel::EntityPropertiesPanel()
	{
		m_noTextureIcon = Texture2D::create("EditorResources/Textures/Icons/EntityPropertiesPanel/NoTextureIcon.png");
	}

	void EntityPropertiesPanel::onImGuiRender()
	{
		ImGui::Begin("Entity Properties");

		if (!m_entity)
		{
			ImGui::End();
			return;
		}

		//Push ID to encapsulate all UI widgets for the current entity
		const UUIDComponent& UUID = m_entity.getComponent<UUIDComponent>();
		const uint64_t UUIDNum = UUID;
		ImGui::PushID(static_cast<int32_t>(UUIDNum));

		//Display input box for the tag component and a button to add new components to the entity
		TagComponent tagComponent = m_entity.getComponent<TagComponent>();
		std::string& tagString = tagComponent;
		char textBuffer[256];
		strcpy_s(textBuffer, sizeof(textBuffer), tagString.c_str());
		if (ImGui::InputText("##Tag", textBuffer, sizeof(textBuffer)))
		{
			tagString = std::string(textBuffer);
		}

		ImVec2 contentRegion = ImGui::GetContentRegionAvail();
		ImVec2 textSize = ImGui::CalcTextSize("Add Component");

		ImGui::SameLine(contentRegion.x + GImGui->Style.FramePadding.x - textSize.x);
		if (ImGui::Button("Add Component"))
		{
			ImGui::OpenPopup("ComponentMenu");
		}

		if (ImGui::BeginPopup("ComponentMenu"))
		{
			if (!m_entity.hasComponent<CameraComponent>())
			{
				if (ImGui::MenuItem("Camera Component"))
				{
					m_entity.addComponent<CameraComponent>();
					ImGui::CloseCurrentPopup();
				}
			}

			if (!m_entity.hasComponent<SpriteComponent>())
			{
				if (ImGui::MenuItem("Sprite Component"))
				{
					m_entity.addComponent<SpriteComponent>();
					ImGui::CloseCurrentPopup();
				}
			}

			ImGui::EndPopup();
		}

		ImGui::Separator();

		//Render section for each component
		componentImGuiRender<UUIDComponent>("UUID Component", CMT_BIND_METHOD(EntityPropertiesPanel::ImGuiRenderUUIDComponentWidget), EntityOptionsFlags::NONE);
		componentImGuiRender<TagComponent>("Tag Component", CMT_BIND_METHOD(EntityPropertiesPanel::ImGuiRenderTagComponentWidget), EntityOptionsFlags::NONE);
		componentImGuiRender<TransformComponent>("Transform Component", CMT_BIND_METHOD(EntityPropertiesPanel::ImGuiRenderTransformComponentWidget), EntityOptionsFlags::NONE);
		componentImGuiRender<CameraComponent>("Camera Component", CMT_BIND_METHOD(EntityPropertiesPanel::ImGuiRenderCameraComponentWidget));
		componentImGuiRender<SpriteComponent>("Sprite Component", CMT_BIND_METHOD(EntityPropertiesPanel::ImGuiRenderSpriteComponentWidget));

		ImGui::PopID();

		ImGui::End();
	}

	template<typename T, typename ComponentUIFunction>
	void EntityPropertiesPanel::componentImGuiRender(const std::string& headerName, ComponentUIFunction componentUIFunction, EntityOptionsFlags optionsFlags)
	{
		if (!m_entity.hasComponent<T>())
			return;

		ImGui::PushID(headerName.c_str());

		const ImGuiTreeNodeFlags treeNodeFlags = ImGuiTreeNodeFlags_DefaultOpen | ImGuiTreeNodeFlags_Framed | ImGuiTreeNodeFlags_SpanAvailWidth | ImGuiTreeNodeFlags_AllowItemOverlap | ImGuiTreeNodeFlags_FramePadding;

		if (optionsFlags == EntityOptionsFlags::NONE)
		{
			if (ImGui::CollapsingHeader(headerName.c_str(), treeNodeFlags))
			{
				T& component = m_entity.getComponent<T>();
				componentUIFunction(component);
			}
		}
		else
		{
			ImVec2 contentRegion = ImGui::GetContentRegionAvail();
			float lineHeight = GImGui->Font->FontSize + (GImGui->Style.FramePadding.y * 2.0f);

			bool opened = ImGui::CollapsingHeader(headerName.c_str(), treeNodeFlags);

			ImGui::SameLine(contentRegion.x - (lineHeight * 0.5f));
			if (ImGui::Button("+", { lineHeight, lineHeight }))
				ImGui::OpenPopup("ComponentSettings");

			bool removeComponent = false;
			if (ImGui::BeginPopup("ComponentSettings"))
			{
				if (optionsFlags & EntityOptionsFlags::CAN_DELETE_ENTITY)
				{
					if (ImGui::MenuItem("Remove Component"))
						removeComponent = true;
				}

				ImGui::EndPopup();
			}

			if (opened)
			{
				T& component = m_entity.getComponent<T>();
				componentUIFunction(component);
			}

			if (removeComponent)
				m_entity.removeComponent<T>();
		}

		ImGui::PopID();
	}

	//
	//UI METHODS FOR EACH COMPONENT
	//

	//TODO: ABSTRACT ATTRIBUTE UI INTO SOME EASY TO USE CLASS
	void EntityPropertiesPanel::ImGuiRenderUUIDComponentWidget(const UUIDComponent& UUID)
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

	void EntityPropertiesPanel::ImGuiRenderTagComponentWidget(TagComponent& tagComponent)
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

	void EntityPropertiesPanel::ImGuiRenderTransformComponentWidget(TransformComponent& transformComponent)
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

	void EntityPropertiesPanel::ImGuiRenderCameraComponentWidget(CameraComponent& cameraComponent)
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

	void EntityPropertiesPanel::ImGuiRenderSpriteComponentWidget(SpriteComponent& spriteComponent)
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
		ImGui::Text("Texture");
		ImGui::NextColumn();
		ImGuiUtilities::popFont();

		ImGui::PushItemWidth(contentColumnWidth);

		int32_t textureRendererID = (spriteComponent.texture) ? static_cast<int32_t>(spriteComponent.texture->getRendererID()) : m_noTextureIcon->getRendererID();

		if (ImGui::ImageButton(reinterpret_cast<void*>(static_cast<uint64_t>((textureRendererID))), { 64.0f, 64.0f }, { 0.0f, 1.0f }, { 1.0f, 0.0f }, 0))
		{
			std::string textureFilepath = PlatformUtilities::openFile("All Picture Files\0*.png;*.jpg\0\0");

			auto texture = Texture2D::create(textureFilepath);
			spriteComponent.texture = texture;
			spriteComponent.subTexture.setTextureAtlas(texture);
		}

		//Set drag drop payload target for opening scene files
		if (ImGui::BeginDragDropTarget())
		{
			const ImGuiPayload* dragDropPayload = nullptr;
			if (dragDropPayload = ImGui::AcceptDragDropPayload("ContentBrowserEntryPathCString"))
			{
				const char* directoryEntryPathCString = reinterpret_cast<const char*>(dragDropPayload->Data);
				std::filesystem::path textureFilepathPath(directoryEntryPathCString);
				std::filesystem::path extension = textureFilepathPath.extension();
				if (!extension.empty())
				{
					if (extension == ".png" || extension == ".jpg")
					{
						auto texture = Texture2D::create(directoryEntryPathCString);
						spriteComponent.texture = texture;
						spriteComponent.subTexture.setTextureAtlas(texture);
					}
				}
			}
			ImGui::EndDragDropTarget();
		}

		//TODO: REMOVE TEXTURE

		ImGui::SameLine();

		if (ImGui::Button("X"))
		{
			spriteComponent.texture = nullptr;
			spriteComponent.subTexture.setTextureAtlas(nullptr);
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
