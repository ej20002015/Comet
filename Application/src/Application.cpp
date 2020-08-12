#include <Comet.h>

#include <iostream>

#include <algorithm>

#include "Comet/Events/KeyboardEvent.h"
#include "Comet/Events/MouseEvent.h"
#include "Comet/Events/ApplicationEvent.h"

class Game : public Comet::Application
{
public:
	Game()
	{
	}

	~Game() override { Comet::Log::clientInfo("Game destroyed"); std::cin.get(); }
};

Comet::Application* Comet::getApplicationInstance()
{
	return new Game;
}