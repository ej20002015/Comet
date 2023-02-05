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

	void setViewportSize(const uint32_t width, const uint32_t height);

	ProjectionType getProjectionType() const { return m_projectionType; }
	void setProjectionType(const ProjectionType projectionType) { m_projectionType = projectionType; recalculateProjection(); }

	float getAspectRatio() const { return m_aspectRatio; }
	void setAspectRatio(const float aspectRatio);

	bool getFixedAspectRatio() const { return m_fixedAspectRatio; }
	void setFixedAspectRatio(const bool fixedAspectRatio) { m_fixedAspectRatio = fixedAspectRatio; }
		
	//Perspective

	void setPerspective(const float FOV = 5.0f, const float nearPlane = 0.001f, const float farPlane = 1000.0f);

	float getPerspectiveFOV() const { return m_perspectiveFOV; }
	void setPerspectiveFOV(const float FOV) { m_perspectiveFOV = FOV; recalculateProjection(); }

	float getPerspectiveNearPlane() const { return m_perspectiveNearPlane; }
	void setPerspectiveNearPlane(const float nearPlane) { m_perspectiveNearPlane = nearPlane; recalculateProjection(); }

	float getPerspectiveFarPlane() const { return m_perspectiveFarPlane; }
	void setPerspectiveFarPlane(const float farPlane) { m_perspectiveFarPlane = farPlane; recalculateProjection(); }

	//Orthographic

	void setOrthographic(const float size = 5.0f, const float nearPlane = -1.0f, const float farPlane = 1.0f);

	float getOrthographicSize() const { return m_orthographicSize; }
	void setOrthographicSize(const float size) { m_orthographicSize = size; recalculateProjection(); }

	float getOrthographicNearPlane() const { return m_orthographicNearPlane; }
	void setOrthographicNearPlane(const float nearPlane) { m_orthographicNearPlane = nearPlane; recalculateProjection(); }

	float getOrthographicFarPlane() const { return m_orthographicFarPlane; }
	void setOrthographicFarPlane(const float farPlane) { m_orthographicFarPlane = farPlane; recalculateProjection(); }

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