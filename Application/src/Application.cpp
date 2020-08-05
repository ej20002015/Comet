#include <Comet.h>

#include <iostream>

class Game : public Comet::Application
{
public:
	Game() { Comet::Log::clientInfo("Welcome to Comet"); };
	~Game() override { Comet::Log::clientInfo("Game destroyed"); std::cin.get(); }

	void run() override { Comet::Log::clientInfo("Running"); Comet::Log::clientInfo("Running"); }
};

Comet::Application* Comet::getApplicationInstance()
{
	return new Game;
}