#pragma once
#include "CometPCH.h"

#include "glm/glm.hpp"

namespace Comet
{

//Camera class just encapsulates a projection matrix - does not include the View matrix (camera transform)
class Camera
{
public:
	Camera() = default;
	Camera(const glm::mat4& projectionMatrix) : m_projectionMatrix(projectionMatrix) {}
	virtual ~Camera() = default;

	const glm::mat4& getProjectionMatrix() const { return m_projectionMatrix; }
	void setProjectionMatrix(const glm::mat4& projectionMatrix) { m_projectionMatrix = projectionMatrix; }

private:
	glm::mat4 m_projectionMatrix = glm::mat4(1.0f);
};

}