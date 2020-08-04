#include "Comet/Comet.h"

#include <iostream>

class Game : public Comet::Application
{
public:
	Game() { std::cout << "Welcome to Comet!" << std::endl; };
	~Game() override { std::cout << "Game destroyed" << std::endl; std::cin.get(); }

	void run() override { std::cout << "Running" << std::endl; }
};

Comet::Application* Comet::getApplicationInstance()
{
	return new Game;
}