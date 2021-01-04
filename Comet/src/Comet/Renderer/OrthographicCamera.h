#pragma once
#include "CometPCH.h"

#include "Comet/Events/Event.h"
#include "Comet/Events/MouseEvent.h"
#include "Comet/Events/KeyboardEvent.h"
#include "Comet/Events/ApplicationEvent.h"
#include "Camera.h"

#include "glm/glm.hpp"

namespace Comet
{

	//Temporary class until we get an editor camera and scene camera
	class OrthographicCamera : public Camera
	{
	public:
		OrthographicCamera();
		OrthographicCamera(float aspectRatio, float zoomLevel = 1.0f);

		void onEvent(Event& e);
		void onUpdate(Timestep ts);

		void setViewMatrix(const glm::mat4& viewMatrix) { m_viewMatrix = viewMatrix; }
		const glm::mat4& getViewMatrix() const { return m_viewMatrix; }

		void setPosition(const glm::vec3& position) { m_position = position; recalculateViewMatrix(); }
		const glm::vec3& getPostition() const { return m_position; }

		void setZoomLevel(float zoomLevel) { m_zoomLevel = zoomLevel; recalculateProjectionMatrix(); }
		float getZoomLevel() const { return m_zoomLevel; }

	private:
		void recalculateViewMatrix();
		void recalculateProjectionMatrix();
		bool onMouseScrolledEvent(MouseScrolledEvent& e);
		bool onWindowResizedEvent(WindowResizedEvent& e);
		
	private:
		float m_aspectRatio;
		float m_zoomLevel;
		glm::mat4 m_viewMatrix = glm::mat4(1.0f);
		glm::vec3 m_position = glm::vec3(0.0f);
		float m_zoomLevelStep = 0.0f;
		float m_movementSpeed = 2.0f;
		float m_zoomSpeed = 50.0f;
	};

}