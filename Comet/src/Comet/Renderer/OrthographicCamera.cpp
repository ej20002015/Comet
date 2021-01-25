#include "CometPCH.h"
#include "OrthographicCamera.h"

#include "Comet/Core/Input.h"

#include "glm/gtc/matrix_transform.hpp"

namespace Comet
{

	OrthographicCamera::OrthographicCamera()
		: m_aspectRatio(16.0f / 9.0f), m_zoomLevel(1.0f), Camera(glm::ortho(-m_zoomLevel * m_aspectRatio, m_zoomLevel* m_aspectRatio, -m_zoomLevel, m_zoomLevel))
	{
	}

	OrthographicCamera::OrthographicCamera(float aspectRatio, float zoomLevel)
		: m_aspectRatio(aspectRatio), m_zoomLevel(zoomLevel), Camera(glm::ortho(-zoomLevel * aspectRatio, zoomLevel * aspectRatio, -zoomLevel, zoomLevel))
	{
	}


	void OrthographicCamera::onEvent(Event& e)
	{
		EventDispatcher dispatcher(e);
		dispatcher.dispatch<MouseScrolledEvent>(CMT_BIND_FUNCTION(OrthographicCamera::onMouseScrolledEvent));
		dispatcher.dispatch<WindowResizedEvent>(CMT_BIND_FUNCTION(OrthographicCamera::onWindowResizedEvent));
	}

	void OrthographicCamera::onUpdate(Timestep ts)
	{
		//Set camera position

		//Poll key presses and move camera (scale speed by zoom level)
		if (Input::isKeyPressed(KeyCode::KEY_A))
			m_position.x -= m_movementSpeed * m_zoomLevel * ts;
		else if (Input::isKeyPressed(KeyCode::KEY_D))
			m_position.x += m_movementSpeed * m_zoomLevel * ts;

		if (Input::isKeyPressed(KeyCode::KEY_S))
			m_position.y -= m_movementSpeed * m_zoomLevel * ts;
		else if (Input::isKeyPressed(KeyCode::KEY_W))
			m_position.y += m_movementSpeed * m_zoomLevel * ts;

		//Set camera zoom level and scale with current zoom level
		m_zoomLevel += (m_zoomSpeed * m_zoomLevelStep * m_zoomLevel) * ts;

		//Prevent camera going behind z-axis and all geometry appearing to flip
		if (m_zoomLevel < 0.005f)
			m_zoomLevel = 0.005f;

		//Reset steps
		m_zoomLevelStep = 0;

		//update matrices
		recalculateProjectionMatrix();
		recalculateViewMatrix();
	}

	void OrthographicCamera::onResize(float aspectRatio)
	{
		m_aspectRatio = aspectRatio;
		recalculateProjectionMatrix();
	}

	void OrthographicCamera::recalculateViewMatrix()
	{
		m_viewMatrix = glm::inverse(glm::translate(glm::mat4(1.0f), m_position));
	}

	void OrthographicCamera::recalculateProjectionMatrix()
	{
		setProjectionMatrix(glm::ortho(-m_zoomLevel * m_aspectRatio, m_zoomLevel * m_aspectRatio, -m_zoomLevel, m_zoomLevel));
	}

	bool OrthographicCamera::onMouseScrolledEvent(MouseScrolledEvent& e)
	{
		m_zoomLevelStep -= e.getYOffset();
		return true;
	}

	bool OrthographicCamera::onWindowResizedEvent(WindowResizedEvent& e)
	{
		m_aspectRatio = static_cast<float>(e.getWidth()) / static_cast<float>(e.getHeight());
		recalculateProjectionMatrix();
		return false;
	}

}
