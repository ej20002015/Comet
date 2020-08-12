#pragma once

#include "CometPCH.h"

#include "Window.h"

namespace Comet
{

	class Application
	{
	public:
		Application();
		virtual ~Application();

		virtual void run();

	private:
		std::unique_ptr<Window> m_window;
		bool m_running;
	};

	//TO BE DEFINED BY CLIENT PROGRAMS
	Application* getApplicationInstance();

}