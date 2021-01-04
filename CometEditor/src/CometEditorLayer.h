#pragma once

#include <Comet.h>

namespace Comet
{

	class CometEditorLayer : public Layer
	{
	public:
		CometEditorLayer() : Layer("Comet Editor Layer") {}
		~CometEditorLayer();

		void onAttach() override;
		void onDetach() override;
		void onUpdate(Timestep ts) override;
		void onImGuiRender() override;
		void onEvent(Event& e) override;

	private:
		Timestep m_ts;
		OrthographicCamera m_orthographicCamera;
		Reference<Texture2DAtlas> m_textureAtlas;
		std::unordered_map<std::string, Reference<Texture2DSubTexture>> m_subTextures;
		Reference<Framebuffer> m_framebuffer;
	};

}