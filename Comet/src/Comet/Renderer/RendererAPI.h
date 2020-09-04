#pragma once
#include "CometPCH.h"

#include "glm/glm.hpp"

namespace Comet
{

    using RendererID = uint32_t;

    enum class RendererAPIType
    {
        NONE,
        OPENGL
    };

    enum class PrimitiveType
    {
        NONE, TRIANGLES, LINES
    };

    struct RendererAPICapabilities
    {
        std::string vendor;
        std::string renderer;
        std::string version;

        int maxSamples = 0;
        float maxAnisotropy = 0.0f;
        int maxTextureUnits = 0;
    };

    //For use by renderer class - not direct use by clients

    class RendererAPI
    {
    public:
        RendererAPI(const RendererAPI&) = delete;

        static void init();
        static void shutdown();

        static void drawIndexed(uint32_t count, PrimitiveType primitive, bool depthTest = true);

        static void setClearColor(const glm::vec4& color);
        static void clear();

        static const RendererAPICapabilities& getCapabilities();

        static RendererAPIType getCurrrentRendererAPIType() { return s_currentRendererAPIType; }

    protected:
        RendererAPI() = default;

        virtual void i_init() = 0;
        virtual void i_shutdown() = 0;

        virtual void i_drawIndexed(uint32_t count, PrimitiveType primitive, bool depthTest = true) = 0;

        virtual void i_setClearColor(const glm::vec4& color) = 0;
        virtual void i_clear() = 0;

    protected:
        static RendererAPICapabilities s_rendererAPICapabilities;

    private:
        static Unique<RendererAPI> s_instance;
        static RendererAPIType s_currentRendererAPIType;
    };

}