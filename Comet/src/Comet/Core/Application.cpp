#include "CometPCH.h"
#include "Application.h"

#include "Comet/Events/Event.h"
#include "glad/glad.h"
#include "Input.h"
#include "Comet/Renderer/Renderer.h"
#include "Comet/Renderer/Framebuffer.h"
#include "Comet/ImGui/ImGuiRenderer.h"

namespace Comet
{

Application* Application::s_instance = nullptr;

Application::Application(const std::string_view windowTitle)
	: m_running(true)
{
	CMT_COMET_ASSERT_MESSAGE(!s_instance, "Application already exists");
	s_instance = this;

	initialiseWindow(windowTitle);

	m_ImGuiLayer = new ImGuiLayer;
	pushOverlay(m_ImGuiLayer);

	Renderer::init();
}

void Application::initialiseWindow(const std::string_view windowTitle)
{
	Window::WindowProperties windowProperties;
	windowProperties.title = windowTitle;
	m_window = Window::create(windowProperties);
	m_window->setEventCallback(CMT_BIND_METHOD(Application::onEvent));
	m_window->setVSync(false);
}

Application::~Application()
{
	Renderer::shutdown();
}

void Application::run()
{
	while (m_running)
	{
		updateTimestep();

		Renderer::clear();
		updateLayers();
		renderImGui();
			
		m_window->onUpdate(m_ts);
	}
}

void Application::renderImGui()
{
	ImGuiRenderer::beginFrame();
	for (auto layer : m_layerStack)
		layer->onImGuiRender();
	ImGuiRenderer::endFrame();
}

void Application::updateLayers()
{
	for (auto layer : m_layerStack)
		layer->onUpdate(m_ts);
}

void Application::updateTimestep()
{
	float currentTime = m_window->getTime();
	m_ts = currentTime - m_timeAtLastFrame;
	m_timeAtLastFrame = currentTime;
}

void Application::onEvent(Event& e)
{
	EventDispatcher dispatcher(e);

	dispatcher.dispatch<WindowClosedEvent>(CMT_BIND_METHOD(Application::onWindowClosedEvent));
	dispatcher.dispatch<WindowResizedEvent>(CMT_BIND_METHOD(Application::onWindowResizedEvent));
		
	//If handled by Application class then return
	if (e.handled)
		return;

	//Propagate events down the layer stack in reverse order (from the top item on the stack downwards - overlays to layers)
	for (auto& layer : m_layerStack | std::views::reverse)
	{
		layer->onEvent(e);
		//If the layer is set to not block events, then set the event handled boolean to false regardless
		if (!layer->getBlocking())
			e.handled = false;
		//If event has been handled then do not pass it to the other layers
		if (e.handled)
			break;
	}
}

void Application::pushLayer(Layer* const layer)
{
	m_layerStack.pushLayer(layer);
}

void Application::popLayer(Layer* const layer)
{
	m_layerStack.popLayer(layer);
}

void Application::pushOverlay(Layer* const overlay)
{
	m_layerStack.pushOverlay(overlay);
}

void Application::popOverlay(Layer* const overlay)
{
	m_layerStack.popOverlay(overlay);
}

bool Application::onWindowClosedEvent(WindowClosedEvent& e)
{
	m_running = false;
	return true;
}

bool Application::onWindowResizedEvent(WindowResizedEvent& e)
{
	m_window->getRenderingContext().onResize(e.getWidth(), e.getHeight());
		
	for (auto& framebuffer : FramebufferPool::getGlobalPool())
		framebuffer->onWindowResize(e.getWidth(), e.getHeight());

	return false;
}

}