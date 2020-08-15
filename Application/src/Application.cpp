#include <Comet.h>

#include <iostream>

#include "ExampleLayer.h"

class Game : public Comet::Application
{
public:
	Game()
	{
		Comet::Layer* layer = new ExampleLayer;
		pushLayer(layer);
		popLayer(layer);
		delete layer;

		pushOverlay(new Comet::ImGuiLayer);
	}

	~Game() override { Comet::Log::clientInfo("Game destroyed"); }
};

Comet::Application* Comet::getApplicationInstance()
{
	return new Game;
}