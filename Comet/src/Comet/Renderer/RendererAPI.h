#pragma once
#include "CometPCH.h"

#include "glm/glm.hpp"

namespace Comet
{

using RendererID = uint32_t;

//For use by renderer class - not direct use by clients

class RendererAPI
{
public:

    enum class Type
    {
        NONE,
        OPENGL
    };

    enum class PrimitiveType
    {
        NONE, TRIANGLES, LINES
    };

    struct Capabilities
    {
        std::string vendor;
        std::string renderer;
        std::string version;

        int maxSamples = 0;
        float maxAnisotropy = 0.0f;
        int maxTextureUnits = 0;
    };

public:
    RendererAPI(const RendererAPI&) = delete;
    virtual ~RendererAPI() = default;

    static void init();
    static void shutdown();

    static void drawIndexed(const uint32_t count, const PrimitiveType primitive, const bool depthTest = true);

    static void setClearColor(const glm::vec4& color);
    static void clear();

    static bool getBackfaceCulling();
    static void setBackfaceCulling(const bool culling);

    static const Capabilities& getCapabilities();

    static Type getCurrrentRendererAPIType() { return s_currentRendererAPIType; }

protected:
    RendererAPI() = default;

    virtual void i_init() = 0;
    virtual void i_shutdown() = 0;

    virtual void i_drawIndexed(const uint32_t count, const PrimitiveType primitive, const bool depthTest = true) = 0;

    virtual void i_setClearColor(const glm::vec4& color) = 0;
    virtual void i_clear() = 0;

    virtual bool i_getBackfaceCulling() const = 0;
    virtual void i_setBackfaceCulling(const bool culling) = 0;

protected:
    static Capabilities s_rendererAPICapabilities;

private:
    static Unique<RendererAPI> s_instance;
    static Type s_currentRendererAPIType;
};

}