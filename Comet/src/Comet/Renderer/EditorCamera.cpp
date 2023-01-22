#include "CometPCH.h"
#include "EditorCamera.h"

#include "glm/gtc/matrix_transform.hpp"

#include "Comet/Core/Input.h"

namespace Comet
{


void EditorCamera::onUpdate(const Timestep ts)
{
	if (Input::isKeyPressed(KeyCode::KEY_LEFT_ALT))
	{
		const glm::vec2 currentMousePosition = Input::getMousePosition();
		// Do not need to multiply by the timestep as this value is encoded implicitly in the mouse delta anyway
		const glm::vec2 mousePositionDelta = currentMousePosition - m_lastMousePosition;
		m_lastMousePosition = currentMousePosition;

		if (Input::isMouseButtonPressed(MouseButtonCode::MOUSE_BUTTON_LEFT))
			mousePan(mousePositionDelta);
		else if (Input::isMouseButtonPressed(MouseButtonCode::MOUSE_BUTTON_RIGHT))
			mouseZoom(mousePositionDelta.y);
		else if (Input::isMouseButtonPressed(MouseButtonCode::MOUSE_BUTTON_MIDDLE))
			mouseRotate(mousePositionDelta);

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
	constexpr float scrollToMouseZoomFactor = -20.0f;
	const float delta = e.getYOffset() * scrollToMouseZoomFactor;
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

void EditorCamera::mouseZoom(const float delta)
{
	m_distance += delta * getMouseZoomSpeed();
	if (m_distance < 0.1f)
		m_distance = 0.1f;
}

glm::vec2 EditorCamera::getMousePanSpeed() const
{
	glm::vec2 panSpeed;
	panSpeed.x = m_distance * (1 / m_viewportSize.x) * PAN_SPEED;
	panSpeed.y = m_distance * (1 / m_viewportSize.y) * PAN_SPEED;

	return getShiftModifiedSpeed(panSpeed);
}

float EditorCamera::getMouseRotateSpeed() const
{
	return getShiftModifiedSpeed(ROTATE_SPEED);
}

float EditorCamera::getMouseZoomSpeed() const
{
	return getShiftModifiedSpeed(ZOOM_SPEED);
}

template<typename T>
T EditorCamera::getShiftModifiedSpeed(const T speed) const
{
	return Input::isKeyPressed(KeyCode::KEY_LEFT_SHIFT) ? SHIFT_SPEED_FACTOR * speed : speed;
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