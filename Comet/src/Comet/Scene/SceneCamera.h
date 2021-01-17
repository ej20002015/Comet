#pragma once
#include "CometPCH.h"

#include "Comet/Renderer/Camera.h"

namespace Comet
{

	class SceneCamera : public Camera
	{
	public:

		enum class ProjectionType { PERSPECTIVE, ORTHOGRAPHIC };

		SceneCamera();
		~SceneCamera() = default;

		void setViewportSize(uint32_t width, uint32_t height);

		ProjectionType getProjectionType() const { return m_projectionType; }

		float getAspectRatio() const { return m_aspectRatio; }
		void setAspectRatio(float aspectRatio);

		bool getFixedAspectRatio() const { return m_fixedAspectRatio; }
		void setFixedAspectRatio(bool fixedAspectRatio) { m_fixedAspectRatio = fixedAspectRatio; }
		
		//Perspective
		void setPerspective(float FOV = 5.0f, float nearPlane = 0.001f, float farPlane = 1000.0f);

		float getPerspectiveFOV() const { return m_perspectiveFOV; }
		void setPerspectiveFOV(float FOV) { m_perspectiveFOV = FOV; }

		float getPerspectiveNearPlane() const { return m_perspectiveNearPlane; }
		void setPerspectiveNearPlane(float nearPlane) { m_perspectiveNearPlane = nearPlane; }

		float getPerspectiveFarPlane() const { return m_perspectiveFarPlane; }
		void setPerspectiveFarPlane(float farPlane) { m_perspectiveFarPlane = farPlane; }

		//Orthographic
		void setOrthographic(float size = 5.0f, float nearPlane = -1.0f, float farPlane = 1.0f);

		float getOrthographicSize() const { return m_orthographicSize; }
		void setOrthographicSize(float size) { m_orthographicSize = size; }

		float getOrthographicNearPlane() const { return m_orthographicNearPlane; }
		void setOrthographicNearPlane(float nearPlane) { m_orthographicNearPlane = nearPlane; }

		float getOrthographicFarPlane() const { return m_orthographicFarPlane; }
		void setOrthographicFarPlane(float farPlane) { m_orthographicFarPlane = farPlane; }

	private:
		void recalculateProjection();

	private:
		ProjectionType m_projectionType = ProjectionType::ORTHOGRAPHIC;

		float m_aspectRatio = 1.0f;
		bool m_fixedAspectRatio = false;

		//Perspective Data
		float m_perspectiveFOV = 45.0f;
		float m_perspectiveNearPlane = 0.001f, m_perspectiveFarPlane = 1000.0f;

		//Orthographic Data
		float m_orthographicSize = 10.0f;
		float m_orthographicNearPlane = -1.0f, m_orthographicFarPlane = 1.0f;
	};

}