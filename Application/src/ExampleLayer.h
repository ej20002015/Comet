#pragma once

#include <Comet.h>

class ExampleLayer : public Comet::Layer
{
public:
	ExampleLayer() : Layer("ExampleLayer") {}

	void onAttach() override { Comet::Log::clientInfo("{0} attached!", m_name); }
	void onDetach() override { Comet::Log::clientInfo("{0} detached!", m_name); }
	void onUpdate() override { Comet::Log::clientInfo("{0} update!", m_name); }
	void onEvent(Comet::Event& e) override { Comet::Log::clientInfo("{0} event: {1}", m_name, e); }
};
