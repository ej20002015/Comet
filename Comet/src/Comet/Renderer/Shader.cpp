#include "CometPCH.h"
#include "Shader.h"

#include "Platform/OpenGL/OpenGLShader.h"

namespace Comet
{

    bool Shader::optimisation = false;
	std::unordered_map<uint32_t, Reference<UniformBuffer>> Shader::s_uniformBuffers;

    Reference<Shader> Shader::create(const std::string& filepath)
    {
		if (!filepath.size())
		{
			Log::cometError("Shader filepath is empty");
			CMT_COMET_ASSERT(false);
			return nullptr;
		}

        switch (RendererAPI::getCurrrentRendererAPIType())
        {
        case RendererAPIType::OPENGL:
            return createReference<OpenGLShader>(filepath);
            break;

        default:
            CMT_COMET_ASSERT_MESSAGE(false, "Unknown GraphicsAPI");
            return nullptr;
            break;
        }
    }

	void Shader::setUniformBuffer(uint32_t bindingPoint, void* data)
	{
		if (s_uniformBuffers.find(bindingPoint) != s_uniformBuffers.end())
		{
			UniformBuffer& uniformBuffer = *s_uniformBuffers.at(bindingPoint);
			Log::cometTrace("Setting uniform buffer '{0}' data at binding point {1}", uniformBuffer.getDescriptor().getName(), bindingPoint);
			uniformBuffer.setData(data, uniformBuffer.getDescriptor().getSize());
		}
		else
		{
			Log::cometError("No uniform buffer exists at binding point {0}", bindingPoint);
			CMT_COMET_ASSERT(false);
		}
	}

	void Shader::setUniformBuffer(uint32_t bindingPoint, void* data, uint32_t size, uint32_t offset)
	{
		if (s_uniformBuffers.find(bindingPoint) != s_uniformBuffers.end())
		{
			UniformBuffer& uniformBuffer = *s_uniformBuffers.at(bindingPoint);
			Log::cometTrace("Setting uniform buffer '{0}' data at binding point {1}", uniformBuffer.getDescriptor().getName(), bindingPoint);
			uniformBuffer.setData(data, size, offset);
		}
		else
		{
			Log::cometError("No uniform buffer exists at binding point {0}", bindingPoint);
			CMT_COMET_ASSERT(false);
		}
	}
}
