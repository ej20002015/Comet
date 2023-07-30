#pragma once
#include "CometPCH.h"

#include <glm/glm.hpp>

#include "Comet/Panels/Panel.h"
#include "Comet/Renderer/RendererAPI.h"
#include "Comet/Renderer/EditorCamera.h"
#include "Comet/Renderer/Framebuffer.h"
#include "Comet/Scene/Entity.h"

namespace Comet
{

class ViewportPanel : public Panel
{
public:
	DEF_PANEL()

	ViewportPanel() = default;
	ViewportPanel(const RendererID textureID)
		: m_textureID(textureID) {}

	std::pair<bool, int32_t> checkClick(const Framebuffer& framebuffer) const;

	bool isHovered() { return m_isHovered; }

	const glm::vec2& getSize() { return m_size; }

	void setGuizmoOperation(const uint32_t guizmoOperation) { m_guizmoOperation = guizmoOperation; }
	void setFramebufferID(const RendererID textureID) { m_textureID = textureID; }
	void setOnFilepathDropCallback(const std::function<void(std::filesystem::path)> f) { m_onFilepathDropCallback = f; }
	void setMatrices(const glm::mat4& projectionMatrix, const glm::mat4& viewMatrix) { m_projectionMatrix = projectionMatrix; m_viewMatrix = viewMatrix; }
	using GetEntityCallback = std::function<Entity()>;
	void setGetEntityCallback(const GetEntityCallback& getEntityCallback) { m_getEntityCallback = getEntityCallback; }

	bool getGuizmoOperationChangeLocked() const { return m_guizmoOperationChangeLocked; }

	void onImGuiRender() override;

public:
	static constexpr glm::vec2 INITIAL_SIZE = { 1.0f, 1.0f };

private:
	RendererID m_textureID = 0;
	glm::vec2 m_size = INITIAL_SIZE;
	std::function<void(std::filesystem::path)> m_onFilepathDropCallback;
	glm::mat4 m_projectionMatrix;
	glm::mat4 m_viewMatrix;
	GetEntityCallback m_getEntityCallback;
	Entity m_selectedEntity;
	int32_t m_guizmoOperation = -1;
	bool m_isHovered = false;
	glm::vec2 m_mousePosRelToViewport;
	bool m_guizmoOperationChangeLocked = false;
};

class GuizmoOptionsPanel : public Panel
{
public:
	DEF_PANEL()

	void onImGuiRender();

private:
	// TODO: Probs make this specific to each Viewport rather than globally defined for all
	static float s_translateScaleSnapValue, s_rotationSnapValue;

	friend class ViewportPanel;
};

}
