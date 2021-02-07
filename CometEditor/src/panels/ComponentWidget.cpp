#include "ComponentWidget.h"

#include "glm/gtc/type_ptr.hpp"
#include "glm/gtx/quaternion.hpp"
#include "glm/gtx/matrix_decompose.hpp"

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
		bool recalculateTransform = false;

		glm::mat4& transform = transformComponent;
		glm::vec3 scale;
		glm::quat rotationQuaternion;
		glm::vec3 translation;
		glm::vec3 skew;
		glm::vec4 perspective;
		glm::decompose(transform, scale, rotationQuaternion, translation, skew, perspective);
		glm::vec3 rotationEuler = glm::degrees(glm::eulerAngles(rotationQuaternion));

		if (ImGui::DragFloat3("Translation", glm::value_ptr(translation), 0.5f))
			recalculateTransform = true;
		if (ImGui::SliderFloat3("Rotation", glm::value_ptr(rotationEuler), 0.0f, 360.0f))
			recalculateTransform = true;
		if (ImGui::DragFloat3("Scale", glm::value_ptr(scale), 0.1f, 0.001f, 10000.0f))
			recalculateTransform = true;

		if (recalculateTransform)
		{
			//Convert the euler angles rotation back to Quaternions
			glm::quat rotationX = glm::angleAxis(glm::radians(rotationEuler.x), glm::vec3(1.0f, 0.0f, 0.0f));
			glm::quat rotationY = glm::angleAxis(glm::radians(rotationEuler.y), glm::vec3(0.0f, 1.0f, 0.0f));
			glm::quat rotationZ = glm::angleAxis(glm::radians(rotationEuler.z), glm::vec3(0.0f, 0.0f, 1.0f));
			rotationQuaternion = rotationX * rotationY * rotationZ;

			transform = glm::translate(glm::mat4(1.0f), translation) * glm::toMat4(rotationQuaternion) * glm::scale(glm::mat4(1.0f), { scale.x, scale.y, scale.z });
		}

		ImGui::ShowDemoWindow();
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

}
