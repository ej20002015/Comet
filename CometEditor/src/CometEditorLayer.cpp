#include "CometEditorLayer.h"

#include "Panels/ContentBrowserPanel.h"
#include "Panels/SceneStateToolbarPanel.h"
#include "Panels/WindowMenuPanel.h"
#include "Panels/RenderingDeviceInfoPanel.h"
#include "Panels/RendererStatsPanel.h"
#include "Panels/SceneViewportPanels.h"

#include "ImGuizmo.h"

namespace Comet
{

const std::unordered_map<std::string, std::string> CometEditorLayer::SCENE_FILE_FILTER_MAP = { { "Comet Scene", fmt::format("*{0}", SceneSerializer::FILE_EXTENSION) } };

void CometEditorLayer::onAttach()
{
    m_sceneFileFilter = PlatformUtilities::constructFilter(SCENE_FILE_FILTER_MAP);
    m_scene = Scene::create();

    Log::cometInfo(SceneHierarchyPanel::s_registered);

    initRendererItems();
    initPanels();
    initImGuiStyle();
}
    
void CometEditorLayer::onUpdate(const Timestep ts)
{
    //Resize framebuffer and set camera projection if viewport size has changed
    const glm::vec2 viewportSize = m_panelManager.get<ViewportPanel>().getSize();

    const bool isViewportSizeChanged = !(viewportSize.x < 1.0f || viewportSize.y < 1.0f) && (
        static_cast<float>(m_framebuffer->getSpecification().width) != viewportSize.x ||
        static_cast<float>(m_framebuffer->getSpecification().height) != viewportSize.y
    );                           
    if (isViewportSizeChanged)
    {
        m_framebuffer->resize(static_cast<uint32_t>(viewportSize.x), static_cast<uint32_t>(viewportSize.y));
        m_editorCamera.setViewportSize(viewportSize);
        m_scene->onViewportResized(static_cast<uint32_t>(viewportSize.x), static_cast<uint32_t>(viewportSize.y));
    }

    m_framebuffer->bind();
    m_framebuffer->clear();
    //Clear entityID color attachment specifically to -1
    m_framebuffer->clearColorAttachment(1, -1);

    switch (m_sceneState)
    {
        case SceneState::EDIT:
            m_editorCamera.onUpdate(ts);
            m_scene->onEditorUpdate(ts, m_editorCamera);
            break;

        case SceneState::PLAY:
            m_scene->onRuntimeUpdate(ts);
            break;
    }

    m_framebuffer->unbind();
}

void CometEditorLayer::onImGuiRender()
{
    //TODO: MOVE TOWARDS AN API FOR SETTING THE MINIMUM SIZE OF EACH WINDOW INDIVIDUALLY RATHER THAN GLOBALLY

    // Pass data to specific panels

    Entity selectedEntity = m_panelManager.get<SceneHierarchyPanel>().getSelectedEntity();

    m_panelManager.get<ViewportPanel>().setMatrices(m_editorCamera.getProjectionMatrix(), m_editorCamera.getViewMatrix());
    m_panelManager.get<ViewportPanel>().setGuizmoOperation(m_guizmoOperation);

    // Render

    ImGuiUtilities::beginDockspace();
    m_panelManager.render();
    ImGuiUtilities::endDockspace();

    // Read data from specific panels

    m_guizmoOperationChangeLocked = m_panelManager.get<ViewportPanel>().getGuizmoOperationChangeLocked();
}

void CometEditorLayer::onEvent(Event& e)
{
    EventDispatcher dispatcher(e);
    dispatcher.dispatch<KeyPressedEvent>(CMT_BIND_METHOD(CometEditorLayer::onKeyPressedEvent));
    dispatcher.dispatch<MouseButtonPressedEvent>(CMT_BIND_METHOD(CometEditorLayer::onMouseButtonPressedEvent));

    m_editorCamera.onEvent(e);
}

void CometEditorLayer::initRendererItems()
{
    Comet::RendererAPI::setClearColor({ 0.1f, 0.1f, 0.1f, 1.0f });

    Framebuffer::Specification framebufferSpecification = {
        .width = static_cast<uint32_t>(ViewportPanel::INITIAL_SIZE.x),
        .height = static_cast<uint32_t>(ViewportPanel::INITIAL_SIZE.y),
        .clearColor = { 0.1f, 0.1f, 0.1f, 1.0f },
        .colorAttachments = {
            Framebuffer::ColorAttachmentFormat::RGBA16F,
            Framebuffer::ColorAttachmentFormat::R32I
        },
        .resizeOnWindowResize = false
    };

    m_framebuffer = Framebuffer::create(framebufferSpecification);
}

void CometEditorLayer::initPanels()
{
    // Add panels to manager

    m_panelManager.add<SceneHierarchyPanel>();
    m_panelManager.add<EntityPropertiesPanel>();
    m_panelManager.add<ContentBrowserPanel>();
    m_panelManager.add<SceneStateToolbarPanel>();
    m_panelManager.add<WindowMenuBarPanel>();
    m_panelManager.add<RenderingDeviceInfoPanel>();
    m_panelManager.add<RendererStatsPanel>();
    m_panelManager.add<ViewportPanel>();
    m_panelManager.add<GuizmoOptionsPanel>();

    // Set context for specific panels where necessary

    m_panelManager.get<SceneHierarchyPanel>().setScene(m_scene);

    const auto getEntityCallback = CMT_BIND_METHOD_FROM_OBJ(SceneHierarchyPanel::getSelectedEntity, &m_panelManager.get<SceneHierarchyPanel>());

    m_panelManager.get<EntityPropertiesPanel>().setGetEntityCallback(getEntityCallback);

    m_panelManager.get<SceneStateToolbarPanel>().setScenePlayedCallback(CMT_BIND_METHOD(CometEditorLayer::onScenePlay));
    m_panelManager.get<SceneStateToolbarPanel>().setSceneStoppedCallback(CMT_BIND_METHOD(CometEditorLayer::onSceneStop));

    m_panelManager.get<WindowMenuBarPanel>().setMenuEntries({
        { "File", {
            { "New", "Ctrl+N", CMT_BIND_METHOD(newScene) },
            { "Open...", "Ctrl+O", CMT_BIND_METHOD(openScene) },
            { "Save As...", "Ctrl+Shift+S", CMT_BIND_METHOD(saveScene) },
            { "Exit", std::nullopt, CMT_BIND_METHOD(exitApplication) }
        }}
    });

    m_panelManager.get<ViewportPanel>().setGetEntityCallback(getEntityCallback);
    m_panelManager.get<ViewportPanel>().setOnFilepathDropCallback(CMT_BIND_METHOD(openScene));
    m_panelManager.get<ViewportPanel>().setFramebufferID(m_framebuffer->getColorAttachmentRendererID());
}

void CometEditorLayer::initImGuiStyle()
{
    ImGuiUtilities::setDarkModeColors();

    ImGuiUtilities::loadFont("assets/fonts/roboto/Roboto-Regular.ttf", 16.0f, ImGuiUtilities::ImGuiFontType::FONT_NORMAL);
    ImGuiUtilities::loadFont("assets/fonts/roboto/Roboto-Bold.ttf", 16.0f, ImGuiUtilities::ImGuiFontType::FONT_BOLD);
    ImGuiUtilities::loadFont("assets/fonts/roboto/Roboto-Italic.ttf", 16.0f, ImGuiUtilities::ImGuiFontType::FONT_NORMAL_ITALIC);
    ImGuiUtilities::loadFont("assets/fonts/roboto/Roboto-BoldItalic.ttf", 16.0f, ImGuiUtilities::ImGuiFontType::FONT_BOLD_ITALIC);

    ImGuiUtilities::setDefaultFont(ImGuiUtilities::ImGuiFontType::FONT_NORMAL);
}

bool CometEditorLayer::onKeyPressedEvent(KeyPressedEvent& e)
{
    const KeyCode keyCode = e.getKeyCode();

    const bool controlPressed = Input::isKeyPressed(KeyCode::KEY_LEFT_CONTROL) || Input::isKeyPressed(KeyCode::KEY_RIGHT_CONTROL);
    const bool shiftPressed = Input::isKeyPressed(KeyCode::KEY_LEFT_SHIFT) || Input::isKeyPressed(KeyCode::KEY_RIGHT_SHIFT);

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
            newScene();
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

bool CometEditorLayer::onMouseButtonPressedEvent(MouseButtonPressedEvent& e)
{
    if (e.getKeyCode() == MouseButtonCode::MOUSE_BUTTON_LEFT)
    {
        const auto[clicked, pixelValue] = m_panelManager.get<ViewportPanel>().checkClick(*m_framebuffer);
        if (clicked)
        {
            Entity clickedEntity = (pixelValue == -1) ? Entity::Null : Entity(m_scene.get(), pixelValue);
            m_panelManager.get<SceneHierarchyPanel>().setSelectedEntity(clickedEntity);
        }

        return true;
    }

    return false;
}

void CometEditorLayer::newScene()
{
    m_guizmoOperation = -1;
    m_scene = Scene::create();
    m_panelManager.get<SceneHierarchyPanel>().setScene(m_scene);
    m_scene->onViewportResized(static_cast<uint32_t>(m_panelManager.get<ViewportPanel>().getSize().x), static_cast<uint32_t>(m_panelManager.get<ViewportPanel>().getSize().y));
}

void CometEditorLayer::saveScene()
{
    const std::string filepath = PlatformUtilities::saveFile(m_sceneFileFilter, SceneSerializer::FILE_EXTENSION);
    if (!filepath.empty())
        SceneSerializer::serialize(filepath, m_scene);
}

void CometEditorLayer::openScene()
{
    std::string filepath = PlatformUtilities::openFile(m_sceneFileFilter);
    openScene(filepath);
}

void CometEditorLayer::openScene(const std::filesystem::path& path)
{
    std::string filepath = path.string();
    if (!filepath.empty())
    {
        newScene();
        SceneSerializer::deserialize(filepath, m_scene);
    }
}

void CometEditorLayer::onScenePlay()
{
    m_sceneState = SceneState::PLAY;
    m_guizmoOperation = -1;
}

void CometEditorLayer::onSceneStop()
{
    m_sceneState = SceneState::EDIT;
}

void CometEditorLayer::exitApplication()
{
    Application::get().exit();
}

}
