#include "CometEditorLayer.h"

//TEMP
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"

#include "ImGuizmo.h"

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
        Comet::Renderer2D::drawSubTexturedQuad({ 2.0f, -1.0f, 0.1f }, m_subTextures.at("Tree"), { 1.0f, 2.0f });
        */

        //Set font
        ImGuiUtilities::loadFont("assets/fonts/roboto/Roboto-Regular.ttf",    16.0f, ImGuiUtilities::ImGuiFontType::FONT_NORMAL);
        ImGuiUtilities::loadFont("assets/fonts/roboto/Roboto-Bold.ttf",       16.0f, ImGuiUtilities::ImGuiFontType::FONT_BOLD);
        ImGuiUtilities::loadFont("assets/fonts/roboto/Roboto-Italic.ttf",     16.0f, ImGuiUtilities::ImGuiFontType::FONT_NORMAL_ITALIC);
        ImGuiUtilities::loadFont("assets/fonts/roboto/Roboto-BoldItalic.ttf", 16.0f, ImGuiUtilities::ImGuiFontType::FONT_BOLD_ITALIC);
        ImGuiUtilities::setDefaultFont(ImGuiUtilities::ImGuiFontType::FONT_NORMAL);
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
            m_editorCamera.setViewportSize(m_viewportSize);
            m_scene->onViewportResized(static_cast<uint32_t>(m_viewportSize.x), static_cast<uint32_t>(m_viewportSize.y));
        }

        m_framebuffer->bind();
        m_framebuffer->clear();

        m_editorCamera.onUpdate(ts);
        m_scene->onEditorUpdate(ts, m_editorCamera);

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
        ImVec2 originalWindowMinSize = ImGuiUtilities::getMinimumWindowSize();
        ImGuiUtilities::setMinimumWindowSize({ 350.0f, originalWindowMinSize.y });

        // DockSpace
        ImGuiID dockspace_id = ImGui::GetID("MyDockSpace");
        ImGui::DockSpace(dockspace_id, ImVec2(0.0f, 0.0f), ImGuiDockNodeFlags_None);

        ImGuiUtilities::setMinimumWindowSize(originalWindowMinSize);

        ImGuiUtilities::setDarkModeColors();

        if (ImGui::BeginMenuBar())
        {
            if (ImGui::BeginMenu("File"))
            {
                if (ImGui::MenuItem("New", "Ctrl+N"))
                {

                }
                if (ImGui::MenuItem("Open...", "Ctrl+O"))
                    openScene();
                if (ImGui::MenuItem("Save As...", "Ctrl+Shift+S"))
                    saveScene();
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

        //TODO: MOVE VIEWPORT CODE INTO SEPARATE FILE
        //Scene Viewport

        ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2{ 0, 0 });
        ImGui::Begin("Scene Viewport");

        //Work out whether ImGui events should not be blocked
        m_viewportFocused = ImGui::IsWindowFocused();
        m_viewportHovered = ImGui::IsWindowHovered();
        Application::get().getImGuiLayer().setBlocking(!(m_viewportFocused || m_viewportHovered));

        //Get size available for viewport
        ImVec2 viewportSize = ImGui::GetContentRegionAvail();
        m_viewportSize = { viewportSize.x, viewportSize.y };

        ImGui::Image(reinterpret_cast<void*>(static_cast<uint64_t>((m_framebuffer->getColorAttachmentRendererID()))), viewportSize, { 0.0f, 1.0f }, { 1.0f, 0.0f });

        //ImGuizmo
        Entity currentlySelectedEntity = m_sceneHierarchyPanel.getSelectedEntity();

        m_guizmoOperationChangeLocked = false;

        if (currentlySelectedEntity && m_guizmoOperation > -1)
        {
            //TODO: Editor Camera that is always perspective
            ImGuizmo::SetOrthographic(false);
            ImGuizmo::SetDrawlist();
            
            //Set the viewport
            float windowWidth = static_cast<float>(ImGui::GetWindowWidth());
            float windowHeight = static_cast<float>(ImGui::GetWindowHeight());
            ImGuizmo::SetRect(ImGui::GetWindowPos().x, ImGui::GetWindowPos().y, windowWidth, windowHeight);

            //Get Model and View matrices of Editor Camera
            glm::mat4 viewMatrix = m_editorCamera.getViewMatrix();
            const glm::mat4& projectionMatrix = m_editorCamera.getProjectionMatrix();

            //Get selected entity transform
            TransformComponent& currentlySelectedEntityTransformComponent = currentlySelectedEntity.getComponent<TransformComponent>();
            glm::mat4 currentlySelectedEntityTransform = currentlySelectedEntityTransformComponent.getTransform();

            //Determine snap value
            float snap = 0.0f;
            if (Input::isKeyPressed(KeyCode::KEY_LEFT_CONTROL))
                snap = (m_guizmoOperation == static_cast<int32_t>(ImGuizmo::OPERATION::ROTATE)) ? m_rotationSnapValue : m_translateScaleSnapValue;

            float snapValues[3] = { snap, snap, snap };

            ImGuizmo::Manipulate(glm::value_ptr(viewMatrix), glm::value_ptr(projectionMatrix), static_cast<ImGuizmo::OPERATION>(m_guizmoOperation), ImGuizmo::MODE::LOCAL, glm::value_ptr(currentlySelectedEntityTransform), nullptr, (snap) ? snapValues : nullptr);

            //Apply changes if gizmo has been used
            if (ImGuizmo::IsUsing())
            {
                //Lock changing of operation during use
                m_guizmoOperationChangeLocked = true;
                currentlySelectedEntityTransformComponent.setTransform(currentlySelectedEntityTransform);
            }
        }

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
        EventDispatcher dispatcher(e);
        dispatcher.dispatch<KeyPressedEvent>(CMT_BIND_METHOD(CometEditorLayer::onKeyPressedEvent));

        m_editorCamera.onEvent(e);
	}

    bool CometEditorLayer::onKeyPressedEvent(KeyPressedEvent& e)
    {
        KeyCode keyCode = e.getKeyCode();

        bool controlPressed = Input::isKeyPressed(KeyCode::KEY_LEFT_CONTROL) || Input::isKeyPressed(KeyCode::KEY_RIGHT_CONTROL);
        bool shiftPressed = Input::isKeyPressed(KeyCode::KEY_LEFT_SHIFT) || Input::isKeyPressed(KeyCode::KEY_RIGHT_SHIFT);

        switch (keyCode)
        {
        case KeyCode::KEY_Q:
            if (!m_guizmoOperationChangeLocked)
                m_guizmoOperation = -1;
            break;
        case KeyCode::KEY_W:
            if (!m_guizmoOperationChangeLocked)
                m_guizmoOperation = ImGuizmo::OPERATION::TRANSLATE;
            break;
        case KeyCode::KEY_E:
            if (!m_guizmoOperationChangeLocked)
                m_guizmoOperation = ImGuizmo::OPERATION::ROTATE;
            break;
        case KeyCode::KEY_R:
            if (!m_guizmoOperationChangeLocked)
                m_guizmoOperation = ImGuizmo::OPERATION::SCALE;
            break;
        case KeyCode::KEY_N:
            if (controlPressed)
            {

            }
            break;
        case KeyCode::KEY_O:
            if (controlPressed)
                openScene();
            break;
        case KeyCode::KEY_S:
            if (controlPressed && shiftPressed)
                saveScene();
            break;
        default:
            break;
        }

        return true;
    }

    void CometEditorLayer::saveScene()
    {
        std::string filepath = PlatformUtilities::saveFile("Comet Scene (*.cmtscn)\0*.cmtscn\0\0)", "cmtscn");
        if (!filepath.empty())
            SceneSerializer::serialize(filepath, m_scene);
    }

    void CometEditorLayer::openScene()
    {
        std::string filepath = PlatformUtilities::openFile("Comet Scene (*.cmtscn)\0*.cmtscn\0\0");
        if (!filepath.empty())
        {
            m_guizmoOperation = -1;
            m_entityPropertiesPanel.setEntity(Comet::Entity());
            m_scene = Scene::create();
            m_sceneHierarchyPanel.setScene(m_scene);
            m_scene->onViewportResized(static_cast<uint32_t>(m_viewportSize.x), static_cast<uint32_t>(m_viewportSize.y));

            SceneSerializer::deserialize(filepath, m_scene);
        }
    }

}
