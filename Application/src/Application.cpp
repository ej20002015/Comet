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
		Comet::Log::clientInfo("Welcome to Comet");

		Comet::KeyPressedEvent e(Comet::KeyCode::None, 0);
		Comet::EventDispatcher dispatcher(e);
		dispatcher.dispatch<Comet::KeyPressedEvent>(CMT_BIND_EVENT_FUNCTION(Game::test));
	};

	bool test(Comet::KeyPressedEvent& e) { Comet::Log::clientInfo("TEST"); return true; }

	~Game() override { Comet::Log::clientInfo("Game destroyed"); std::cin.get(); }

	void run() override { Comet::Log::clientInfo("Running"); Comet::Log::clientInfo("Running"); }
};

Comet::Application* Comet::getApplicationInstance()
{
	return new Game;
}