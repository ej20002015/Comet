#include <Comet.h>

#include <iostream>

class Game : public Comet::Application
{
public:
	Game() { CMT_CLIENT_INFO("Welcome to Comet"); };
	~Game() override { CMT_CLIENT_INFO("Game destroyed"); std::cin.get(); }

	void run() override { CMT_CLIENT_INFO("Running"); }
};

Comet::Application* Comet::getApplicationInstance()
{
	return new Game;
}