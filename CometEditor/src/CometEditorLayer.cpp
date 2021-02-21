#include "CometEditorLayer.h"

//TEMP
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"

namespace Comet
{

	Comet::CometEditorLayer::~CometEditorLayer()
	{
	}

	void CometEditorLayer::onAttach()
	{
        Comet::Renderer::setClearColor({ 0.1f, 0.1f, 0.1f, 1.0f });

        m_viewportSize = { m_initialFramebufferSize.x, m_initialFramebufferSize.y };

        m_testTexture = Texture2D::create("assets/textures/container2.png");

        m_textureAtlas = Comet::Texture2D::create("assets/textures/RPGpack_sheet_2X.png", false, Comet::TextureFilter::NEAREST, Comet::TextureFilter::NEAREST);

        /*m_subTextures["Door"] = Comet::Texture2DSubTexture::create(m_textureAtlas, { 14.0f, 0.0f }, { 1.0f, 1.0f });
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
        m_subTextures["Tree"] = Comet::Texture2DSubTexture::create(m_textureAtlas, { 0.0f, 1.0f }, { 1.0f, 2.0f });*/


        //m_orthographicCamera = Comet::OrthographicCamera(m_initialFramebufferSize.x / m_initialFramebufferSize.y, 3.0f);


        FramebufferSpecification framebufferSpecification;
        framebufferSpecification.width = static_cast<uint32_t>(m_initialFramebufferSize.x);
        framebufferSpecification.height = static_cast<uint32_t>(m_initialFramebufferSize.y);
        framebufferSpecification.clearColor = { 0.1f, 0.1f, 0.1f, 1.0f };
        //Since this will be the scene viewport, it should be resized in accordance with the ImGui image's size - not the whole window
        framebufferSpecification.resizeOnWindowResize = false;
       
        m_framebuffer = Framebuffer::create(framebufferSpecification);

        //SCENE STUFF
        m_scene = Scene::create();

        m_sceneHierarchyPanel.setScene(m_scene);

        m_cameraEntity = m_scene->createEntity("camera");
        m_cameraEntity.addComponent<CameraComponent>(true);

        class CameraControllerScript : public EntityNativeScript
        {
        protected:
            void onUpdate(Timestep ts) override
            {
                TransformComponent& transformComponent = getComponent<TransformComponent>();

                if (Input::isKeyPressed(KeyCode::KEY_D))
                    transformComponent.translation.x += m_cameraSpeed * ts;
                else if (Input::isKeyPressed(KeyCode::KEY_A))
                    transformComponent.translation.x -= m_cameraSpeed * ts;

                if (Input::isKeyPressed(KeyCode::KEY_W))
                    transformComponent.translation.y += m_cameraSpeed * ts;
                else if (Input::isKeyPressed(KeyCode::KEY_S))
                    transformComponent.translation.y -= m_cameraSpeed * ts;
            }   

        private:
            const float m_cameraSpeed = 4.0f;
        };

        m_cameraEntity.addComponent<NativeScriptComponent>().bind<CameraControllerScript>();


        //Create entities for tilemap
        
        {
            Entity tilemapEntity = m_scene->createEntity("tilemapEntity1");
            glm::vec3 position = { 0.0f, 0.0f, 0.0f };
            tilemapEntity.getComponent<TransformComponent>().translation = position;
            tilemapEntity.addComponent<SpriteComponent>(m_textureAtlas, glm::vec4(1.0f), 1.0f, SpriteComponent::SpriteTextureType::SUB_TEXTURE, 128, glm::vec2(14.0f, 0.0f ), glm::vec2(1.0f, 1.0f));
        }
        {
            Entity tilemapEntity = m_scene->createEntity("tilemapEntity2");
            glm::vec3 position = { 0.0f, 0.0f, -0.1f };
            tilemapEntity.getComponent<TransformComponent>().translation = position;
            tilemapEntity.addComponent<SpriteComponent>(m_textureAtlas, glm::vec4(1.0f), 1.0f, SpriteComponent::SpriteTextureType::SUB_TEXTURE, 128, glm::vec2(0.0f, 9.0f), glm::vec2(1.0f, 1.0f));
        }
        {
            Entity tilemapEntity = m_scene->createEntity("tilemapEntity3");
            glm::vec3 position = { -1.0f, 0.0f, 0.0f };
            tilemapEntity.getComponent<TransformComponent>().translation = position;
            tilemapEntity.addComponent<SpriteComponent>(m_textureAtlas, glm::vec4(1.0f), 1.0f, SpriteComponent::SpriteTextureType::SUB_TEXTURE, 128, glm::vec2(1.0f, 9.0f), glm::vec2(1.0f, 1.0f));
        }
        {
            Entity tilemapEntity = m_scene->createEntity("tilemapEntity4");
            glm::vec3 position = { 1.0f, 0.0f, 0.0f };
            tilemapEntity.getComponent<TransformComponent>().translation = position;
            tilemapEntity.addComponent<SpriteComponent>(m_textureAtlas, glm::vec4(1.0f), 1.0f, SpriteComponent::SpriteTextureType::SUB_TEXTURE, 128, glm::vec2(3.0f, 9.0f), glm::vec2(1.0f, 1.0f));
        }
        {
            Entity tilemapEntity = m_scene->createEntity("tilemapEntity5");
            glm::vec3 position = { 0.0f, 1.0f, -0.1f };
            tilemapEntity.getComponent<TransformComponent>().translation = position;
            tilemapEntity.addComponent<SpriteComponent>(m_textureAtlas, glm::vec4(1.0f), 1.0f, SpriteComponent::SpriteTextureType::SUB_TEXTURE, 128, glm::vec2(5.0f, 9.0f), glm::vec2(1.0f, 1.0f));
        }
        {
            Entity tilemapEntity = m_scene->createEntity("Window");
            glm::vec3 position = { 0.0f, 1.0f, 0.1f };
            tilemapEntity.getComponent<TransformComponent>().translation = position;
            tilemapEntity.addComponent<SpriteComponent>(m_textureAtlas, glm::vec4(1.0f), 1.0f, SpriteComponent::SpriteTextureType::SUB_TEXTURE, 128, glm::vec2(12.0f, 0.0f), glm::vec2(1.0f, 1.0f));
        }
        {
            Entity tilemapEntity = m_scene->createEntity("tilemapEntity7");
            glm::vec3 position = { -1.0f, 1.0f, 0.0f };
            tilemapEntity.getComponent<TransformComponent>().translation = position;
            tilemapEntity.addComponent<SpriteComponent>(m_textureAtlas, glm::vec4(1.0f), 1.0f, SpriteComponent::SpriteTextureType::SUB_TEXTURE, 128, glm::vec2(4.0f, 9.0f), glm::vec2(1.0f, 1.0f));
        }
        {
            Entity tilemapEntity = m_scene->createEntity("tilemapEntity8");
            glm::vec3 position = { 1.0f, 1.0f, 0.0f };
            tilemapEntity.getComponent<TransformComponent>().translation = position;
            tilemapEntity.addComponent<SpriteComponent>(m_textureAtlas, glm::vec4(1.0f), 1.0f, SpriteComponent::SpriteTextureType::SUB_TEXTURE, 128, glm::vec2(6.0f, 9.0f), glm::vec2(1.0f, 1.0f));
        }
        {
            Entity tilemapEntity = m_scene->createEntity("tilemapEntity12");
            glm::vec3 position = { 0.0f, 3.0f, 0.1f };
            tilemapEntity.getComponent<TransformComponent>().translation = position;
            tilemapEntity.getComponent<TransformComponent>().scale = { 3.0f, 3.0f, 1.0f };
            tilemapEntity.addComponent<SpriteComponent>(m_textureAtlas, glm::vec4(1.0f), 1.0f, SpriteComponent::SpriteTextureType::SUB_TEXTURE, 128, glm::vec2(2.0f, 4.0f), glm::vec2(2.0f, 3.0f));
        }
        {
            Entity tilemapEntity = m_scene->createEntity("tilemapEntity9");
            glm::vec3 position = { -1.0f, 2.0f, -0.1f };
            tilemapEntity.getComponent<TransformComponent>().translation = position;
            tilemapEntity.addComponent<SpriteComponent>(m_textureAtlas, glm::vec4(1.0f), 1.0f, SpriteComponent::SpriteTextureType::SUB_TEXTURE, 128, glm::vec2(5.0f, 9.0f), glm::vec2(1.0f, 1.0f));
        }
        {
            Entity tilemapEntity = m_scene->createEntity("tilemapEntity10");
            glm::vec3 position = { 0.0f, 2.0f, -0.1f };
            tilemapEntity.getComponent<TransformComponent>().translation = position;
            tilemapEntity.addComponent<SpriteComponent>(m_textureAtlas, glm::vec4(1.0f), 1.0f, SpriteComponent::SpriteTextureType::SUB_TEXTURE, 128, glm::vec2(5.0f, 9.0f), glm::vec2(1.0f, 1.0f));
        }
        {
            Entity tilemapEntity = m_scene->createEntity("tilemapEntity11");
            glm::vec3 position = { 1.0f, 2.0f, -0.1f };
            tilemapEntity.getComponent<TransformComponent>().translation = position;
            tilemapEntity.addComponent<SpriteComponent>(m_textureAtlas, glm::vec4(1.0f), 1.0f, SpriteComponent::SpriteTextureType::SUB_TEXTURE, 128, glm::vec2(5.0f, 9.0f), glm::vec2(1.0f, 1.0f));
        }



        //Create test sprite
        {
            Entity spriteEntity = m_scene->createEntity("spriteEntity1");
            glm::vec3 position = { -5.0f, 1.0f, 0.0f };
            spriteEntity.getComponent<TransformComponent>().translation = position;
            spriteEntity.addComponent<SpriteComponent>(m_testTexture);
        }


        /*

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

       Comet::Renderer2D::drawSubTexturedQuad({ -2.0f, -1.0f, 0.1f }, m_subTextures.at("Tree"), { 1.0f, 2.0f });
       Comet::Renderer2D::drawSubTexturedQuad({ 2.0f, -1.0f, 0.1f }, m_subTextures.at("Tree"), { 1.0f, 2.0f });*/

	}

	void CometEditorLayer::onDetach()
	{
	}
    
	void CometEditorLayer::onUpdate(Timestep ts)
	{
        m_ts = ts;

        //Only allow inputs to work and change camera position when on the viewport
        /*if (m_viewportFocused && m_viewportHovered)
            m_orthographicCamera.onUpdate(ts);*/

        //Resize framebuffer and set camera projection if viewport size has changed
        if (!(m_viewportSize.x < 1.0f || m_viewportSize.y < 1.0f) &&
           (static_cast<float>(m_framebuffer->getSpecification().width) != m_viewportSize.x || static_cast<float>(m_framebuffer->getSpecification().height) != m_viewportSize.y))
        {
            m_framebuffer->resize(static_cast<uint32_t>(m_viewportSize.x), static_cast<uint32_t>(m_viewportSize.y));
            //m_orthographicCamera.onResize(m_viewportSize.x / m_viewportSize.y);
            m_scene->onViewportResized(static_cast<uint32_t>(m_viewportSize.x), static_cast<uint32_t>(m_viewportSize.y));
        }

        m_framebuffer->bind();
        m_framebuffer->clear();

        m_scene->onUpdate(ts);

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
        ImGui::Begin("DockSpace", nullptr, window_flags);
        ImGui::PopStyleVar(3);

        //ImGui styling
        ImGuiStyle& style = ImGui::GetStyle();
        ImVec2 originalWindowMinSize = style.WindowMinSize;
        style.WindowMinSize.x = 350.0f;

        // DockSpace
        ImGuiID dockspace_id = ImGui::GetID("MyDockSpace");
        ImGui::DockSpace(dockspace_id, ImVec2(0.0f, 0.0f), ImGuiDockNodeFlags_None);

        style.WindowMinSize = originalWindowMinSize;

        setDarkThemeColours();

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

        ImGui::Image(reinterpret_cast<void*>(static_cast<uint64_t>((m_framebuffer->getColorAttachmentRendererID()))), { m_viewportSize.x, m_viewportSize.y }, { 0.0f, 1.0f }, { 1.0f, 0.0f });

        ImGui::End();
        ImGui::PopStyleVar();

        //Scene Hierarchy Panel
        m_sceneHierarchyPanel.onImGuiRender();

        //Entity Properties Panel
        Entity selectedEntity = m_sceneHierarchyPanel.getSelectedEntity();
        m_entityPropertiesPanel.setEntity(selectedEntity);
        m_entityPropertiesPanel.onImGuiRender();

        ImGui::End();
	}

	void CometEditorLayer::onEvent(Event& e)
	{
        //m_orthographicCamera.onEvent(e);
	}

    //TODO: change colors and move into some sort of util function in the main engine
    void CometEditorLayer::setDarkThemeColours()
    {
        ImVec4* colors = ImGui::GetStyle().Colors;

        colors[ImGuiCol_WindowBg] = ImVec4{ 0.1f, 0.105f, 0.11f, 1.0f };

        //Headers
        colors[ImGuiCol_Header] = ImVec4{ 0.2f, 0.205f, 0.21f, 1.0f };
        colors[ImGuiCol_HeaderHovered] = ImVec4{ 0.3f, 0.305f, 0.31f, 1.0f };
        colors[ImGuiCol_HeaderActive] = ImVec4{ 0.15f, 0.1505f, 0.151f, 1.0f };

        //Buttons
        colors[ImGuiCol_Button] = ImVec4{ 0.2f, 0.205f, 0.21f, 1.0f };
        colors[ImGuiCol_ButtonHovered] = ImVec4{ 0.3f, 0.305f, 0.31f, 1.0f };
        colors[ImGuiCol_ButtonActive] = ImVec4{ 0.15f, 0.1505f, 0.151f, 1.0f };

        //Frame Background
        colors[ImGuiCol_FrameBg] = ImVec4{ 0.2f, 0.205f, 0.21f, 1.0f };
        colors[ImGuiCol_FrameBgHovered] = ImVec4{ 0.3f, 0.305f, 0.31f, 1.0f };
        colors[ImGuiCol_FrameBgActive] = ImVec4{ 0.15f, 0.1505f, 0.151f, 1.0f };

        //Tabs
        colors[ImGuiCol_Tab] = ImVec4{ 0.15f, 0.1505f, 0.151f, 1.0f };
        colors[ImGuiCol_TabHovered] = ImVec4{ 0.38f, 0.3805f, 0.381f, 1.0f };
        colors[ImGuiCol_TabActive] = ImVec4{ 0.28f, 0.2805f, 0.281f, 1.0f };
        colors[ImGuiCol_TabUnfocused] = ImVec4{ 0.15f, 0.1505f, 0.151f, 1.0f };
        colors[ImGuiCol_TabUnfocusedActive] = ImVec4{ 0.2f, 0.205f, 0.21f, 1.0f };

        //Title Background
        colors[ImGuiCol_TitleBg] = ImVec4{ 0.1f, 0.105f, 0.11f, 1.0f };
        colors[ImGuiCol_TitleBgActive] = ImVec4{ 0.15f, 0.1505f, 0.151f, 1.0f };
        colors[ImGuiCol_TitleBgCollapsed] = ImVec4{ 0.95f, 0.1505f, 0.951f, 1.0f };
    }

}
