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
		UUIDComponent& UUID = m_entity.getComponent<UUIDComponent>();
		uint64_t UUIDNum = UUID.ID;
		ImGui::PushID(static_cast<int32_t>(UUIDNum));

		//Display input box for the tag component and a button to add new components to the entity

		TagComponent& tagComponent = m_entity.getComponent<TagComponent>();
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

		//
		//UI CODE FOR EACH COMPONENT
		//
		componentImGuiRender<UUIDComponent>("UUID Component", [](const UUIDComponent& UUID)
		{
			ImGuiUtilities::beginPropertyGrid();
			ImGuiUtilities::property("UUID", UUID.ID);
			ImGuiUtilities::endPropertyGrid();
		},
		EntityOptionsFlags::NONE);

		componentImGuiRender<TagComponent>("Tag Component", [](TagComponent& tagComponent)
		{
			ImGuiUtilities::beginPropertyGrid();
			ImGuiUtilities::property("Tag", tagComponent.tag);
			ImGuiUtilities::endPropertyGrid();
		},
		EntityOptionsFlags::NONE);

		componentImGuiRender<TransformComponent>("Transform Component", [](TransformComponent& transformComponent)
		{
			ImGuiUtilities::beginPropertyGrid();

			ImGuiUtilities::property("Translation", transformComponent.translation, 0.01f, "%.2f");

			glm::vec3 rotationEuler = glm::degrees(glm::eulerAngles(transformComponent.rotation));
			if (ImGuiUtilities::property("Rotation", rotationEuler))
			{
				//Change euler angles back to quaternions
				glm::quat rotationX = glm::angleAxis(glm::radians(rotationEuler.x), glm::vec3(1.0f, 0.0f, 0.0f));
				glm::quat rotationY = glm::angleAxis(glm::radians(rotationEuler.y), glm::vec3(0.0f, 1.0f, 0.0f));
				glm::quat rotationZ = glm::angleAxis(glm::radians(rotationEuler.z), glm::vec3(0.0f, 0.0f, 1.0f));
				transformComponent.rotation = rotationZ * rotationY * rotationX;
			}

			ImGuiUtilities::property("Scale", transformComponent.scale, 0.01f, "%.02f");

			ImGuiUtilities::endPropertyGrid();
		},
		EntityOptionsFlags::NONE);

		componentImGuiRender<CameraComponent>("Camera Component", [](CameraComponent& cameraComponent)
		{
			SceneCamera& sceneCamera = cameraComponent.camera;

			ImGuiUtilities::beginPropertyGrid();

			ImGuiUtilities::property("Primary Camera", cameraComponent.primary);

			ImGuiUtilities::seperator();

			const char* projectionTypes[] = { "Perspective", "Orthographic" };
			auto projectionType = sceneCamera.getProjectionType();

			if (ImGuiUtilities::property("Projection Type", projectionTypes, 2, projectionType))
				sceneCamera.setProjectionType(projectionType);

			ImGuiUtilities::seperator();

			//Only display the relevant fields for the current projection type
			if (sceneCamera.getProjectionType() == SceneCamera::ProjectionType::PERSPECTIVE)
			{
				float perspectiveFOV = sceneCamera.getPerspectiveFOV();
				if (ImGuiUtilities::property("Perspective Verticle FOV", perspectiveFOV))
					sceneCamera.setPerspectiveFOV(perspectiveFOV);

				float perspectiveNearPlane = sceneCamera.getPerspectiveNearPlane();
				if (ImGuiUtilities::property("Perspective Near Plane", perspectiveNearPlane))
					sceneCamera.setPerspectiveNearPlane(perspectiveNearPlane);

				float perspectiveFarPlane = sceneCamera.getPerspectiveFarPlane();
				if (ImGuiUtilities::property("Perspective Far Plane", perspectiveFarPlane))
					sceneCamera.setPerspectiveFarPlane(perspectiveFarPlane);
			}
			else
			{
				float orthographicSize = sceneCamera.getOrthographicSize();
				if (ImGuiUtilities::property("Orthographic Size", orthographicSize))
					sceneCamera.setOrthographicSize(orthographicSize);

				float orthographicNearPlane = sceneCamera.getOrthographicNearPlane();
				if (ImGuiUtilities::property("Orthographic Near Plane", orthographicNearPlane))
					sceneCamera.setOrthographicNearPlane(orthographicNearPlane);

				float orthographicFarPlane = sceneCamera.getOrthographicFarPlane();
				if (ImGuiUtilities::property("Orthographic Far Plane", orthographicFarPlane))
					sceneCamera.setOrthographicFarPlane(orthographicFarPlane);
			}

			ImGuiUtilities::seperator();

			bool fixedAspectRatio = sceneCamera.getFixedAspectRatio();
			if (ImGuiUtilities::property("Fixed Aspect Ratio", fixedAspectRatio))
				sceneCamera.setFixedAspectRatio(fixedAspectRatio);

			if (sceneCamera.getFixedAspectRatio())
			{
				float aspectRatio = sceneCamera.getAspectRatio();
				if (ImGuiUtilities::property("Aspect Ratio", aspectRatio))
					sceneCamera.setAspectRatio(aspectRatio);
			}

			ImGuiUtilities::endPropertyGrid();
		});

		componentImGuiRender<SpriteComponent>("Sprite Component", [this](SpriteComponent& spriteComponent)
		{
			ImGuiUtilities::beginPropertyGrid();

			ImGuiUtilities::propertyColorPicker("Color", spriteComponent.color);

			ImGuiUtilities::seperator();

			int32_t textureRendererID = (spriteComponent.texture) ? static_cast<int32_t>(spriteComponent.texture->getRendererID()) : m_noTextureIcon->getRendererID();

			if (ImGuiUtilities::propertyImageButton("Texture", textureRendererID, { 64.0f, 64.0f }))
			{
				std::string textureFilepath = PlatformUtilities::openFile("All Picture Files\0*.png;*.jpg\0\0");

				if (!textureFilepath.empty())
				{
					auto texture = Texture2D::create(textureFilepath);
					spriteComponent.texture = texture;
					spriteComponent.subTexture.setTextureAtlas(texture);
				}
			}

			//Set drag drop payload target for opening texture files
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

			if (ImGuiUtilities::propertyButton("Clear Texture", "X"))
			{
				spriteComponent.texture = nullptr;
				spriteComponent.subTexture.setTextureAtlas(nullptr);
			}
				
			if (spriteComponent.texture)
			{
				ImGuiUtilities::seperator();

				ImGuiUtilities::property("Tiling Factor", spriteComponent.tilingFactor);

				const char* textureTypes[] = { "Normal", "Sub-Texture" };
				auto& textureType = spriteComponent.spriteTextureType;

				ImGuiUtilities::property("Texture Type", textureTypes, 2, textureType);

				if (spriteComponent.spriteTextureType == SpriteComponent::SpriteTextureType::SUB_TEXTURE)
				{
					Texture2DSubTexture& subTexture = spriteComponent.subTexture;

					ImGuiUtilities::seperator();

					int32_t cellSize = static_cast<int32_t>(subTexture.getCellSize());
					if (ImGuiUtilities::property("Cell Size", cellSize, 1.0f, "%d", 0, 1024))
						subTexture.setCellSize(cellSize);

					glm::vec2 textureAtlasIndex = subTexture.getTextureAtlasIndex();
					if (ImGuiUtilities::property("Texture Atlas Index", textureAtlasIndex, 1.0f, "%.0f", 0.0f, 1024.0f))
						subTexture.setTextureAtlasIndex(textureAtlasIndex);

					glm::vec2 textureScale = subTexture.getTextureScale();
					if (ImGuiUtilities::property("Texture Scale", textureScale, 0.1f, "%.1f", 0.1f, 100.0f))
						subTexture.setTextureScale(textureScale);
				}
			}

			ImGuiUtilities::endPropertyGrid();
		});

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

}
