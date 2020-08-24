#pragma once
#include "CometPCH.h"

namespace Comet
{

    using RendererID = uint32_t;

    enum class RendererAPIType
    {
        NONE,
        OPENGL
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

    class RendererAPI
    {
    public:
        RendererAPI(const RendererAPI&) = delete;

        static void init();
        static void shutdown();

        static void setClearColor(float r, float g, float b, float a);

        static const RendererAPICapabilities& getCapabilities();

        static RendererAPIType getCurrrentRendererAPIType() { return s_currentRendererAPIType; }

    protected:
        RendererAPI() = default;

        virtual void i_init() = 0;
        virtual void i_shutdown() = 0;

        virtual void i_setClearColor(float r, float g, float b, float a) = 0;

    protected:
        static RendererAPICapabilities s_rendererAPICapabilities;

    private:
        static Unique<RendererAPI> s_instance;
        static RendererAPIType s_currentRendererAPIType;
    };

}