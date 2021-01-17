#include "CometEditorLayer.h"

namespace Comet
{

	Comet::CometEditorLayer::~CometEditorLayer()
	{
	}

	void CometEditorLayer::onAttach()
	{
        Comet::Renderer::setClearColor({ 0.1f, 0.1f, 0.1f, 1.0f });

        m_viewportSize = { m_initialFramebufferSize.x, m_initialFramebufferSize.y };

        m_textureAtlas = Comet::Texture2DAtlas::create("assets/textures/RPGpack_sheet_2X.png", 128, false, Comet::TextureFilter::NEAREST, Comet::TextureFilter::NEAREST);

        m_subTextures["Door"] = Comet::Texture2DSubTexture::create(m_textureAtlas, { 14.0f, 0.0f }, { 1.0f, 1.0f });
        m_subTextures["BottomWall"] = Comet::Texture2DSubTexture::create(m_textureAtlas, { 0.0f, 9.0f }, { 1.0f, 1.0f });
        m_subTextures["LeftCornerWall"] = Comet::Texture2DSubTexture::create(m_textureAtlas, { 1.0f, 9.0f }, { 1.0f, 1.0f });
        m_subTextures["RightCornerWall"] = Comet::Texture2DSubTexture::create(m_textureAtlas, { 3.0f, 9.0f }, { 1.0f, 1.0f });
        m_subTextures["Wall"] = Comet::Texture2DSubTexture::create(m_textureAtlas, { 5.0f, 9.0f }, { 1.0f, 1.0f });
        m_subTextures["SmallWindow"] = Comet::Texture2DSubTexture::create(m_textureAtlas, { 12.0f, 0.0f }, { 1.0f, 1.0f });
        m_subTextures["LeftWall"] = Comet::Texture2DSubTexture::create(m_textureAtlas, { 4.0f, 9.0f }, { 1.0f, 1.0f });
        m_subTextures["RightWall"] = Comet::Texture2DSubTexture::create(m_textureAtlas, { 6.0f, 9.0f }, { 1.0f, 1.0f });
        m_subTextures["Roof"] = Comet::Texture2DSubTexture::create(m_textureAtlas, { 2.0f, 4.0f }, { 2.0f, 3.0f });
        m_subTextures["GrassDirtTop"] = Comet::Texture2DSubTexture::create(m_textureAtlas, { 1.0f, 12.0f }, { 1.0f, 1.0f });
        m_subTextures["GrassDirtBottom"] = Comet::Texture2DSubTexture::create(m_textureAtlas, { 1.0f, 10.0f }, { 1.0f, 1.0f });
        m_subTextures["GrassDirtLeft"] = Comet::Texture2DSubTexture::create(m_textureAtlas, { 0.0f, 11.0f }, { 1.0f, 1.0f });
        m_subTextures["GrassDirtRight"] = Comet::Texture2DSubTexture::create(m_textureAtlas, { 2.0f, 11.0f }, { 1.0f, 1.0f });
        m_subTextures["GrassDirtBottomLeft"] = Comet::Texture2DSubTexture::create(m_textureAtlas, { 0.0f, 10.0f }, { 1.0f, 1.0f });
        m_subTextures["GrassDirtBottomRight"] = Comet::Texture2DSubTexture::create(m_textureAtlas, { 2.0f, 10.0f }, { 1.0f, 1.0f });
        m_subTextures["GrassDirtTopRight"] = Comet::Texture2DSubTexture::create(m_textureAtlas, { 2.0f, 12.0f }, { 1.0f, 1.0f });
        m_subTextures["GrassDirtTopLeft"] = Comet::Texture2DSubTexture::create(m_textureAtlas, { 0.0f, 12.0f }, { 1.0f, 1.0f });
        m_subTextures["Grass"] = Comet::Texture2DSubTexture::create(m_textureAtlas, { 1.0f, 11.0f }, { 1.0f, 1.0f });
        m_subTextures["Tree"] = Comet::Texture2DSubTexture::create(m_textureAtlas, { 0.0f, 1.0f }, { 1.0f, 2.0f });


        m_orthographicCamera = Comet::OrthographicCamera(m_initialFramebufferSize.x / m_initialFramebufferSize.y, 3.0f);


        FramebufferSpecification framebufferSpecification;
        framebufferSpecification.width = static_cast<uint32_t>(m_initialFramebufferSize.x);
        framebufferSpecification.height = static_cast<uint32_t>(m_initialFramebufferSize.y);
        framebufferSpecification.clearColor = { 0.1f, 0.1f, 0.1f, 1.0f };
        //Since this will be the scene viewport, it should be resized in accordance with the ImGui image's size - not the whole window
        framebufferSpecification.resizeOnWindowResize = false;
       
        m_framebuffer = Framebuffer::create(framebufferSpecification);

        //SCENE STUFF
        m_scene = Scene::create();

        Entity testEntity = m_scene->createEntity();
        Log::clientInfo(static_cast<std::string>(testEntity.getComponent<UUIDComponent>()));
        m_cameraEntity = m_scene->createEntity("camera");
        m_cameraEntity.addComponent<CameraComponent>();
	}

	void CometEditorLayer::onDetach()
	{
	}
    
	void CometEditorLayer::onUpdate(Timestep ts)
	{
        m_ts = ts;

        //Only allow inputs to work and change camera position when on the viewport
        if (m_viewportFocused && m_viewportHovered)
            m_orthographicCamera.onUpdate(ts);

        //Resize framebuffer and set camera projection if viewport size has changed
        if (!(m_viewportSize.x < 1.0f || m_viewportSize.y < 1.0f) &&
           (static_cast<float>(m_framebuffer->getSpecification().width) != m_viewportSize.x || static_cast<float>(m_framebuffer->getSpecification().height) != m_viewportSize.y))
        {
            m_framebuffer->resize(static_cast<uint32_t>(m_viewportSize.x), static_cast<uint32_t>(m_viewportSize.y));
            m_orthographicCamera.onResize(m_viewportSize.x / m_viewportSize.y);
            m_scene->onViewportResized(static_cast<uint32_t>(m_viewportSize.x), static_cast<uint32_t>(m_viewportSize.y));
        }

        m_framebuffer->bind();
        m_framebuffer->clear();

        SceneCamera& sceneCamera = m_cameraEntity.getComponent<CameraComponent>().camera;
        TransformComponent cameraTransform = m_cameraEntity.getComponent<TransformComponent>();

        //Comet::Renderer2D::beginScene(m_orthographicCamera, glm::inverse(m_orthographicCamera.getViewMatrix()), false);
        Comet::Renderer2D::beginScene(sceneCamera, cameraTransform, false);

        //Draw basic map
        Comet::Renderer2D::drawSubTexturedQuad({ 0.0f,  0.0f, -0.1f }, m_subTextures.at("BottomWall"));
        Comet::Renderer2D::drawSubTexturedQuad({ 0.0f,  0.0f }, m_subTextures.at("Door"));
        Comet::Renderer2D::drawSubTexturedQuad({ -1.0f,  0.0f }, m_subTextures.at("LeftCornerWall"));
        Comet::Renderer2D::drawSubTexturedQuad({ 1.0f,  0.0f }, m_subTextures.at("RightCornerWall"));
        Comet::Renderer2D::drawSubTexturedQuad({ 0.0f,  1.0f, -0.1f }, m_subTextures.at("Wall"));
        Comet::Renderer2D::drawSubTexturedQuad({ 0.0f,  1.0f }, m_subTextures.at("SmallWindow"));
        Comet::Renderer2D::drawSubTexturedQuad({ -1.0f,  1.0f }, m_subTextures.at("LeftWall"));
        Comet::Renderer2D::drawSubTexturedQuad({ 1.0f,  1.0f }, m_subTextures.at("RightWall"));
        Comet::Renderer2D::drawSubTexturedQuad({ -1.0f,  2.0f }, m_subTextures.at("Wall"));
        Comet::Renderer2D::drawSubTexturedQuad({ 0.0f,  2.0f }, m_subTextures.at("Wall"));
        Comet::Renderer2D::drawSubTexturedQuad({ 1.0f,  2.0f }, m_subTextures.at("Wall"));

        Comet::Renderer2D::drawSubTexturedQuad({ -2.0f,  -2.0f }, m_subTextures.at("GrassDirtBottomLeft"));
        Comet::Renderer2D::drawSubTexturedQuad({ 2.0f,  -2.0f }, m_subTextures.at("GrassDirtBottomRight"));
        Comet::Renderer2D::drawSubTexturedQuad({ -1.0f,  -2.0f }, m_subTextures.at("GrassDirtBottom"));
        Comet::Renderer2D::drawSubTexturedQuad({ 0.0f,  -2.0f }, m_subTextures.at("GrassDirtBottom"));
        Comet::Renderer2D::drawSubTexturedQuad({ 1.0f,  -2.0f }, m_subTextures.at("GrassDirtBottom"));
        Comet::Renderer2D::drawSubTexturedQuad({ -1.0f,  -1.0f }, m_subTextures.at("GrassDirtTop"));
        Comet::Renderer2D::drawSubTexturedQuad({ 0.0f,  -1.0f }, m_subTextures.at("GrassDirtTop"));
        Comet::Renderer2D::drawSubTexturedQuad({ 1.0f,  -1.0f }, m_subTextures.at("GrassDirtTop"));
        Comet::Renderer2D::drawSubTexturedQuad({ 2.0f,  -1.0f }, m_subTextures.at("GrassDirtRight"));
        Comet::Renderer2D::drawSubTexturedQuad({ -2.0f,  -1.0f }, m_subTextures.at("GrassDirtLeft"));
        Comet::Renderer2D::drawSubTexturedQuad({ -2.0f,   0.0f }, m_subTextures.at("GrassDirtLeft"));
        Comet::Renderer2D::drawSubTexturedQuad({ 2.0f,   0.0f }, m_subTextures.at("GrassDirtRight"));
        Comet::Renderer2D::drawSubTexturedQuad({ -2.0f,   0.0f }, m_subTextures.at("GrassDirtLeft"));
        Comet::Renderer2D::drawSubTexturedQuad({ 2.0f,   0.0f }, m_subTextures.at("GrassDirtRight"));
        Comet::Renderer2D::drawSubTexturedQuad({ -2.0f,   1.0f }, m_subTextures.at("GrassDirtLeft"));
        Comet::Renderer2D::drawSubTexturedQuad({ 2.0f,   1.0f }, m_subTextures.at("GrassDirtRight"));
        Comet::Renderer2D::drawSubTexturedQuad({ -2.0f,   2.0f }, m_subTextures.at("GrassDirtLeft"));
        Comet::Renderer2D::drawSubTexturedQuad({ 2.0f,   2.0f }, m_subTextures.at("GrassDirtRight"));
        Comet::Renderer2D::drawSubTexturedQuad({ -2.0f,   3.0f }, m_subTextures.at("GrassDirtLeft"));
        Comet::Renderer2D::drawSubTexturedQuad({ 2.0f,   3.0f }, m_subTextures.at("GrassDirtRight"));
        Comet::Renderer2D::drawSubTexturedQuad({ -2.0f,   4.0f }, m_subTextures.at("GrassDirtLeft"));
        Comet::Renderer2D::drawSubTexturedQuad({ 2.0f,   4.0f }, m_subTextures.at("GrassDirtRight"));
        Comet::Renderer2D::drawSubTexturedQuad({ -1.0f,   4.0f }, m_subTextures.at("Grass"));
        Comet::Renderer2D::drawSubTexturedQuad({ 0.0f,   4.0f }, m_subTextures.at("Grass"));
        Comet::Renderer2D::drawSubTexturedQuad({ 1.0f,   4.0f }, m_subTextures.at("Grass"));
        Comet::Renderer2D::drawSubTexturedQuad({ -2.0f,   5.0f }, m_subTextures.at("GrassDirtTopLeft"));
        Comet::Renderer2D::drawSubTexturedQuad({ 2.0f,   5.0f }, m_subTextures.at("GrassDirtTopRight"));
        Comet::Renderer2D::drawSubTexturedQuad({ -1.0f,   5.0f }, m_subTextures.at("GrassDirtTop"));
        Comet::Renderer2D::drawSubTexturedQuad({ 0.0f,   5.0f }, m_subTextures.at("GrassDirtTop"));
        Comet::Renderer2D::drawSubTexturedQuad({ 1.0f,   5.0f }, m_subTextures.at("GrassDirtTop"));

        Comet::Renderer2D::drawSubTexturedQuad({ 0.0f,  3.0f, 0.1f }, m_subTextures.at("Roof"), { 3.0f, 3.0f });

        Comet::Renderer2D::drawSubTexturedQuad({ -2.0f, -1.0f, 0.1f }, m_subTextures.at("Tree"), { 1.0f, 2.0f });
        Comet::Renderer2D::drawSubTexturedQuad({ 2.0f, -1.0f, 0.1f }, m_subTextures.at("Tree"), { 1.0f, 2.0f });

        Comet::Renderer2D::endScene();

        m_framebuffer->unbind();
	}

	void CometEditorLayer::onImGuiRender()
	{
        // We are using the ImGuiWindowFlags_NoDocking flag to make the parent window not dockable into,
        // because it would be confusing to have two docking targets within each others.
        ImGuiWindowFlags window_flags = ImGuiWindowFlags_MenuBar | ImGuiWindowFlags_NoDocking;
        ImGuiViewport* viewport = ImGui::GetMainViewport();
        ImGui::SetNextWindowPos(viewport->GetWorkPos());
        ImGui::SetNextWindowSize(viewport->GetWorkSize());
        ImGui::SetNextWindowViewport(viewport->ID);
        ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
        ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);
        window_flags |= ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove;
        window_flags |= ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoNavFocus;

        // Important: note that we proceed even if Begin() returns false (aka window is collapsed).
        // This is because we want to keep our DockSpace() active. If a DockSpace() is inactive,
        // all active windows docked into it will lose their parent and become undocked.
        // We cannot preserve the docking relationship between an active window and an inactive docking, otherwise
        // any change of dockspace/settings would lead to windows being stuck in limbo and never being visible.
        ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));
        ImGui::Begin("DockSpace Demo", nullptr, window_flags);
        ImGui::PopStyleVar(3);

        // DockSpace
        ImGuiID dockspace_id = ImGui::GetID("MyDockSpace");
        ImGui::DockSpace(dockspace_id, ImVec2(0.0f, 0.0f), ImGuiDockNodeFlags_None);

        if (ImGui::BeginMenuBar())
        {
            if (ImGui::BeginMenu("File"))
            {
                if (ImGui::MenuItem("Exit"))
                    Application::get().exit();

                ImGui::EndMenu();
            }

            ImGui::EndMenuBar();
        }

        //Dockable windows lie within dockspace code

        ImGui::Begin("Rendering Device Information");

        ImGui::Separator();

        ImGui::Text("Renderer Capabilites");

        ImGui::Separator();

        ImGui::Text("Vendor: %s", Comet::RendererAPI::getCapabilities().vendor.c_str());
        ImGui::Text("Renderer: %s", Comet::RendererAPI::getCapabilities().renderer.c_str());
        ImGui::Text("Version: %s", Comet::RendererAPI::getCapabilities().version.c_str());

        ImGui::Text("");

        ImGui::Text("Max Samples: %d", Comet::RendererAPI::getCapabilities().maxSamples);
        ImGui::Text("Max Anisotropy: %f", Comet::RendererAPI::getCapabilities().maxAnisotropy);
        ImGui::Text("Max Texture Units: %d", Comet::RendererAPI::getCapabilities().maxTextureUnits);

        ImGui::End();

        ImGui::Begin("Renderer2D Statistics");

        ImGui::Text("Frametime");

        ImGui::Separator();

        ImGui::Text("%fms (%f.2 fps)", m_ts.getMilliseconds(), 1.0f / m_ts.getSeconds());

        ImGui::Separator();

        auto stats = Comet::Renderer2D::getRenderer2DStats();

        ImGui::Text("%u quads", stats.quads);
        ImGui::Text("%u vertices", stats.getVertices());
        ImGui::Text("%u triangles", stats.getTriangles());
        ImGui::Text("%u draw calls", stats.drawCalls);

        ImGui::End();

        //Scene Viewport

        ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2{ 0, 0 });
        ImGui::Begin("Scene Viewport");

        //Work out whether ImGui events should not be blocked
        m_viewportFocused = ImGui::IsWindowFocused();
        m_viewportHovered = ImGui::IsWindowHovered();
        Application::get().getImGuiLayer().setBlocking(!(m_viewportFocused && m_viewportHovered));

        //Get size available for viewport
        m_viewportSize = ImGui::GetContentRegionAvail();

        ImGui::Image(reinterpret_cast<void*>(m_framebuffer->getColorAttachmentRendererID()), { m_viewportSize.x, m_viewportSize.y }, { 0.0f, 1.0f }, { 1.0f, 0.0f });

        ImGui::End();
        ImGui::PopStyleVar();

        ImGui::End();
	}

	void CometEditorLayer::onEvent(Event& e)
	{
        m_orthographicCamera.onEvent(e);
	}

}
