#include "CometPCH.h"
#include "Shader.h"

#include "Platform/OpenGL/OpenGLShader.h"

namespace Comet
{

    bool Shader::optimisation = false;

    Reference<Shader> Shader::create(const std::string& filepath)
    {
        CMT_COMET_ASSERT_MESSAGE(filepath.size(), "filepath is empty");

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

}
