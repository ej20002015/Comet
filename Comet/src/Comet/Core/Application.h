#pragma once

namespace Comet
{

	class Application
	{
	public:
		Application();
		virtual ~Application();

		virtual void run() = 0;

	private:
		Application* instance;
	};

	//TO BE DEFINED BY CLIENT PROGRAMS
	Application* getApplicationInstance();

}