#include "CometPCH.h"
#include "OrthographicCamera.h"

#include "glm/gtc/matrix_transform.hpp"

namespace Comet
{

	OrthographicCamera::OrthographicCamera(float aspectRatio, float zoomLevel)
		: m_aspectRatio(aspectRatio), m_zoomLevel(zoomLevel), Camera(glm::ortho(-zoomLevel * aspectRatio, zoomLevel * aspectRatio, -zoomLevel, zoomLevel))
	{
		memset(m_movementSteps, 0, sizeof(m_movementSteps));
	}


	void OrthographicCamera::onEvent(Event& e)
	{
		EventDispatcher dispatcher(e);
		dispatcher.dispatch<MouseScrolledEvent>(CMT_BIND_EVENT_FUNCTION(OrthographicCamera::onMouseScrolledEvent));
		dispatcher.dispatch<KeyPressedEvent>(CMT_BIND_EVENT_FUNCTION(OrthographicCamera::onKeyPressedEvent));
		dispatcher.dispatch<WindowResizedEvent>(CMT_BIND_EVENT_FUNCTION(OrthographicCamera::onWindowResizedEvent));
	}

	void OrthographicCamera::onUpdate(Timestep ts)
	{
		//Set camera position

		for (int i = 0; i < 4; i++)
		{
			if (m_movementSteps[i])
				Log::cometTrace("Not 0");
		}	

		float xChange = (-(m_movementSpeed * m_movementSteps[0]) + (m_movementSpeed * m_movementSteps[1])) * ts;
    	m_position.x += (-(m_movementSpeed * m_movementSteps[0]) + (m_movementSpeed * m_movementSteps[1])) * ts;
		m_position.y += (-(m_movementSpeed * m_movementSteps[2]) + (m_movementSpeed * m_movementSteps[3])) * ts;

		//Set camera zoom level
		m_zoomLevel += (m_zoomSpeed * m_zoomLevelStep) * ts;

		//Reset steps
		memset(m_movementSteps, 0, sizeof(m_movementSteps));
		m_zoomLevelStep = 0;

		//update matrices
		recalculateProjectionMatrix();
		recalculateViewMatrix();
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

	bool OrthographicCamera::onKeyPressedEvent(KeyPressedEvent& e)
	{
		switch (e.getKeyCode())
		{
		case KeyCode::KEY_A:
			m_movementSteps[0]++;
			return true;
			break;
		case KeyCode::KEY_D:
			m_movementSteps[1]++;
			return true;
			break;
		case KeyCode::KEY_S:
			m_movementSteps[2]++;
			return true;
			break;
		case KeyCode::KEY_W:
			m_movementSteps[3]++;
			return true;
			break;
		default:
			break;
		}

		return false;
	}

	bool OrthographicCamera::onWindowResizedEvent(WindowResizedEvent& e)
	{
		m_aspectRatio = static_cast<float>(e.getWidth()) / static_cast<float>(e.getHeight());
		recalculateProjectionMatrix();
		return false;
	}

}
