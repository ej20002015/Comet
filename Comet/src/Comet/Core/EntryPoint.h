#pragma once
#include "CometPCH.h"

#include "Application.h"
#include "Log.h"

int main(int argc, char** argv)
{
	Comet::Log::init();

	Comet::Application* application = Comet::getApplicationInstance();
	application->run();
	delete application;

	return 0;
}