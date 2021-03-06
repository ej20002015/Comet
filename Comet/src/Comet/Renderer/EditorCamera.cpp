#include "CometPCH.h"
#include "EditorCamera.h"

#include "glm/gtc/matrix_transform.hpp"

#include "Comet/Core/Input.h"

namespace Comet
{


	void EditorCamera::onUpdate(Timestep ts)
	{
		if (Input::isKeyPressed(KeyCode::KEY_LEFT_ALT))
		{
			glm::vec2 currentMousePosition = Input::getMousePosition();
			glm::vec2 mousePositionDelta = (currentMousePosition - m_lastMousePosition) * ts.getSeconds();
			m_lastMousePosition = currentMousePosition;

			if (Input::isMouseButtonPressed(MouseButtonCode::MOUSE_BUTTON_1))
			{
				mousePan(mousePositionDelta);
			}
			else if (Input::isMouseButtonPressed(MouseButtonCode::MOUSE_BUTTON_3))
			{
				mouseRotate(mousePositionDelta);
			}
			else if (Input::isMouseButtonPressed(MouseButtonCode::MOUSE_BUTTON_2))
			{
				mouseZoom(mousePositionDelta.y);
			}

			recalculateViewMatrix();
		}
	}

	void EditorCamera::onEvent(Event& e)
	{
		EventDispatcher dispatcher(e);
		dispatcher.dispatch<MouseScrolledEvent>(CMT_BIND_METHOD(EditorCamera::onMouseScrolledEvent));
	}

	bool EditorCamera::onMouseScrolledEvent(MouseScrolledEvent& e)
	{
		float delta = e.getYOffset() * -0.2f;
		mouseZoom(delta);
		recalculateViewMatrix();
		return true;
	}

	void EditorCamera::recalculateProjectionMatrix()
	{
		m_aspectRatio = m_viewportSize.x / m_viewportSize.y;
		setProjectionMatrix(glm::perspective(m_FOV, m_aspectRatio, m_nearPlane, m_farPlane));
	}

	void EditorCamera::recalculateViewMatrix()
	{
		recalculatePosition();
		m_viewMatrix = glm::inverse(glm::translate(glm::mat4(1.0f), m_position) * glm::toMat4(getOrientation()));
	}

	void EditorCamera::recalculatePosition()
	{
		m_position = m_focusPoint - (getForwardDirection() * m_distance);
	}

	void EditorCamera::mousePan(const glm::vec2& delta)
	{
		m_focusPoint += getRightDirection() * delta.x * getMousePanSpeed().x * 10.0f * -1.0f;
		m_focusPoint += getUpDirection() * delta.y * getMousePanSpeed().y * 10.0f;
	}

	void EditorCamera::mouseRotate(const glm::vec2& delta)
	{
		float yawSign = getUpDirection().y < 0 ? -1.0f : 1.0f;
		m_yaw += yawSign * delta.x * getMouseRotateSpeed();
		m_pitch += delta.y * getMouseRotateSpeed();
	}

	void EditorCamera::mouseZoom(float delta)
	{
		m_distance += delta * getMouseZoomSpeed();
		if (m_distance < 0.1f)
			m_distance = 0.1f;
	}

	glm::vec2 EditorCamera::getMousePanSpeed() const
	{
		glm::vec2 panSpeed;
		panSpeed.x = m_distance * (1 / m_viewportSize.x) * 100.0f;
		panSpeed.y = m_distance * (1 / m_viewportSize.y) * 100.0f;

		return getShiftModifiedSpeed(panSpeed);
	}

	float EditorCamera::getMouseRotateSpeed() const
	{
		//Consider making this into a const member variable
		constexpr float rotateSpeed = 4.0f;
		return getShiftModifiedSpeed(rotateSpeed);
	}

	float EditorCamera::getMouseZoomSpeed() const
	{
		float zoomSpeed = glm::sqrt(10.0f * m_distance);
		return getShiftModifiedSpeed(zoomSpeed);
	}

	float EditorCamera::getShiftModifiedSpeed(float speed) const
	{
		constexpr float shiftSpeedFactor = 0.25f;
		return Input::isKeyPressed(KeyCode::KEY_LEFT_SHIFT) ? shiftSpeedFactor * speed : speed;
	}

	glm::vec2 EditorCamera::getShiftModifiedSpeed(const glm::vec2& speed) const
	{
		constexpr float shiftSpeedFactor = 0.25f;
		return Input::isKeyPressed(KeyCode::KEY_LEFT_SHIFT) ? shiftSpeedFactor * speed : speed;
	}

	glm::quat EditorCamera::getOrientation() const
	{
		return glm::quat(glm::vec3(m_pitch, m_yaw, 0.0f));
	}

	glm::vec3 EditorCamera::getRightDirection() const
	{
		return glm::rotate(getOrientation(), glm::vec3(1.0f, 0.0f, 0.0f));
	}

	glm::vec3 EditorCamera::getUpDirection() const
	{
		return glm::rotate(getOrientation(), glm::vec3(0.0f, 1.0f, 0.0f));
	}

	glm::vec3 EditorCamera::getForwardDirection() const
	{
		return glm::rotate(getOrientation(), glm::vec3(0.0f, 0.0f, -1.0f));
	}

}