#include "CometPCH.h"
#include "SceneCamera.h"

#include "glm/gtc/matrix_transform.hpp"

namespace Comet
{

	SceneCamera::SceneCamera()
	{
		recalculateProjection();
	}

	void SceneCamera::setViewportSize(uint32_t width, uint32_t height)
	{
		if (m_fixedAspectRatio)
		{
			Log::cometWarn("Camera viewport not changing - has fixed aspect ratio");
			return;
		}

		m_aspectRatio = static_cast<float>(width) / static_cast<float>(height);
		recalculateProjection();
	}

	void SceneCamera::setAspectRatio(float aspectRatio)
	{
		if (!m_fixedAspectRatio)
		{
			Log::cometWarn("Cannot set aspect ratio of camera whose ratio is not fixed");
			return;
		}

		m_aspectRatio = aspectRatio;
	}

	void SceneCamera::setPerspective(float FOV, float nearPlane, float farPlane)
	{
		m_perspectiveFOV = FOV;
		m_perspectiveNearPlane = nearPlane;
		m_perspectiveFarPlane = farPlane;
		m_projectionType = ProjectionType::PERSPECTIVE;
	}

	void SceneCamera::setOrthographic(float size, float nearPlane, float farPlane)
	{
		m_orthographicSize = size;
		m_orthographicNearPlane = nearPlane;
		m_orthographicFarPlane = farPlane;
		m_projectionType = ProjectionType::ORTHOGRAPHIC;
	}

	void SceneCamera::recalculateProjection()
	{
		if (m_projectionType == ProjectionType::PERSPECTIVE)
			setProjectionMatrix(glm::perspective(m_perspectiveFOV, m_aspectRatio, m_perspectiveNearPlane, m_perspectiveFarPlane));
		else
		{
			float left =   -m_orthographicSize * m_aspectRatio * 0.5f;
			float right =   m_orthographicSize * m_aspectRatio * 0.5f;
			float bottom = -m_orthographicSize * 0.5f;
			float top =     m_orthographicSize * 0.5f;

			setProjectionMatrix(glm::ortho(left, right, bottom, top, m_orthographicNearPlane, m_orthographicFarPlane));
		}
	}

}
