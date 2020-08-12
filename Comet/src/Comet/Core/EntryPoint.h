#pragma once

#include "CometPCH.h"

#include "Application.h"
#include "Log.h"

extern Comet::Application* Comet::getApplicationInstance();

int main(int argc, char** argv)
{
	Comet::Log::init();

	Comet::Application* application = Comet::getApplicationInstance();
	application->run();
	delete application;

	return 0;
}