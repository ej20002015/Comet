#pragma once
#include "CometPCH.h"

#include "Renderer.h"

namespace Comet
{

	class Shader
	{
	public:
		virtual ~Shader() = default;

		static Reference<Shader> create(const std::string& filepath);

		virtual void reload() = 0;

		virtual void bind() = 0;

		virtual const std::string& getName() const = 0;
		virtual const std::string& getFilepath() const = 0;
		virtual RendererID getRendererID() const = 0;

	protected:
		static bool optimisation;
	};

}