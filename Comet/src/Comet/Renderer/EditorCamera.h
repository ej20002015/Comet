#pragma once
#include "Camera.h"

#include "glm/glm.hpp"
#define GLM_ENABLE_EXPERIMENTAL
#include "glm/gtx/quaternion.hpp"

#include "Comet/Events/Event.h"
#include "Comet/Events/MouseEvent.h"

namespace Comet
{

	class EditorCamera : public Camera
	{
	public:
		EditorCamera() { recalculateProjectionMatrix(); recalculateViewMatrix(); }

		void onUpdate(Timestep ts);

		void onEvent(Event& e);

		bool onMouseScrolledEvent(MouseScrolledEvent& e);


		float getFOV() const { return m_FOV; }
		void setFOV(float FOV) { m_FOV = FOV; recalculateProjectionMatrix(); }

		float getAspectRatio() const { return m_aspectRatio; }

		float getNearPlane() const { return m_nearPlane; }
		void setNearPlane(float nearPlane) { m_nearPlane = nearPlane; recalculateProjectionMatrix(); }

		float getFarPlane() const { return m_farPlane; }
		void setFarPlane(float farPlane) { m_farPlane = farPlane; recalculateProjectionMatrix(); }

		void setViewportSize(const glm::vec2& viewportSize) { m_viewportSize = viewportSize; recalculateProjectionMatrix(); }


		const glm::vec3& getPosition() const { return m_position; }
		void setPosition(const glm::vec3& position) { m_position = position; recalculateViewMatrix(); }

		const glm::vec3& getFocusPoint() const { return m_focusPoint; }
		void setFocusPoint(const glm::vec3& focusPoint) { m_focusPoint = focusPoint; recalculateViewMatrix(); }

		float getPitch() const { return m_pitch; }

		float getYaw() const { return m_yaw; }


		glm::mat4 getViewProjectionMatrix() const { return getProjectionMatrix() * m_viewMatrix; }

		const glm::mat4& getViewMatrix() const { return m_viewMatrix; }

	private:
		void recalculateProjectionMatrix();	
		void recalculateViewMatrix();

		void recalculatePosition();

		void mousePan(const glm::vec2& delta);
		void mouseRotate(const glm::vec2& delta);
		void mouseZoom(float delta);

		glm::vec2 getMousePanSpeed() const;
		float getMouseRotateSpeed() const;
		float getMouseZoomSpeed() const;

		float getShiftModifiedSpeed(float speed) const;
		glm::vec2 getShiftModifiedSpeed(const glm::vec2& speed) const;

		glm::quat getOrientation() const;
		glm::vec3 getRightDirection() const;
		glm::vec3 getUpDirection() const;
		glm::vec3 getForwardDirection() const;

	private:
		float m_FOV = 45.0f, m_aspectRatio = 16.0f / 9.0f, m_nearPlane = 0.001f, m_farPlane = 1000.0f;
		glm::vec2 m_viewportSize = { 1280.0f, 720.0f };

		glm::vec3 m_position = { 0.0f, 0.0f, 0.0f }, m_focusPoint = { 0.0f, 0.0f, 0.0f };
		float m_pitch = 0.0f, m_yaw = 0.0f;
		//Scalar value that determines the distance between the position of the camera and the focal point (scales the forward direction)
		float m_distance = 10.0f;

		glm::vec2 m_lastMousePosition = { 0.0f, 0.0f };

		glm::mat4 m_viewMatrix = glm::mat4(1.0f);
	};

}