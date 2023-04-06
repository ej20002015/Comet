#pragma once
#include <Comet.h>

#include "CometEditorLayer.h"
#include "CometEditorResourceManager.h"

namespace Comet
{

	class CometEditorApplication : public Application
	{
	public:
		CometEditorApplication()
			: Application("Comet Editor")
		{
			CometEditorResourceManager::init();
			pushLayer(new CometEditorLayer());
			Log::setLevel(Log::LogLevel::CMT_INFO);
		}

		~CometEditorApplication() = default;
	};

	DEF_COMET_APP(CometEditorApplication)

}