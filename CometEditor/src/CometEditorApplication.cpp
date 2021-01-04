#pragma once
#include <Comet.h>

#include "CometEditorLayer.h"

namespace Comet
{

	class CometEditorApplication : public Application
	{
	public:
		CometEditorApplication()
			: Application("Comet Editor")
		{
			pushLayer(new CometEditorLayer());
		}

		~CometEditorApplication() = default;
	};

	Comet::Application* Comet::getApplicationInstance()
	{
		return new CometEditorApplication;
	}

}