#pragma once

#include "Application.h"

extern Comet::Application* Comet::getApplicationInstance();

int main(int argc, char** argv)
{
	Comet::Application* application = Comet::getApplicationInstance();
	application->run();
	delete application;
}