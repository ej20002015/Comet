#pragma once

extern Comet::Application* Comet::getApplicationInstance();

int main(int argc, char** argv)
{
	Comet::Log::init();

	Comet::Application* application = Comet::getApplicationInstance();
	application->run();
	delete application;

	return 0;
}