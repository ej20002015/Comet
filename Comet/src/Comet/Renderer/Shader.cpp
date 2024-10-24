#include "CometPCH.h"
#include "Shader.h"

#include "Platform/OpenGL/OpenGLShader.h"

namespace Comet
{

std::unordered_map<uint32_t, Reference<UniformBuffer>> Shader::s_uniformBuffers;

Reference<Shader> Shader::create(const std::filesystem::path& filepath)
{
	if (filepath.empty())
		throw CometException() << "Shader filepath cannot be empty";

    switch (RendererAPI::getCurrentRendererAPIType())
    {
	case RendererAPI::Type::OPENGL:
        return createReference<OpenGLShader>(filepath);
        break;

    default:
        CMT_COMET_ASSERT_MESSAGE(false, "Unknown GraphicsAPI");
        return nullptr;
        break;
    }
}

void Shader::setUniformBuffer(const uint32_t bindingPoint, const void* const data)
{
	CMT_COMET_ASSERT_MESSAGE(s_uniformBuffers.find(bindingPoint) != s_uniformBuffers.end(), fmt::format("No uniform buffer exists at binding point {0}", bindingPoint));

	UniformBuffer& uniformBuffer = *s_uniformBuffers.at(bindingPoint);
	Log::cometTrace("Setting uniform buffer '{0}' data at binding point {1}", uniformBuffer.getDescriptor().getName(), bindingPoint);
	uniformBuffer.setData(data);
}

void Shader::setUniformBuffer(const uint32_t bindingPoint, const void* const data, const uint32_t size, const uint32_t offset)
{
	CMT_COMET_ASSERT_MESSAGE(s_uniformBuffers.find(bindingPoint) != s_uniformBuffers.end(), fmt::format("No uniform buffer exists at binding point {0}", bindingPoint));

	UniformBuffer& uniformBuffer = *s_uniformBuffers.at(bindingPoint);
	Log::cometTrace("Setting uniform buffer '{0}' data at binding point {1}", uniformBuffer.getDescriptor().getName(), bindingPoint);
	uniformBuffer.setData(data, size, offset);
}

}
