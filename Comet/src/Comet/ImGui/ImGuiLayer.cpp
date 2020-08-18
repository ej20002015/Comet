#include "CometPCH.h"
#include "ImGuiLayer.h"

#include "imgui.h"
#include "Platform/OpenGL/ImGuiOpenGLRenderer.h"

#include "Comet/Core/Application.h"

//TODO: TEMPORARY
#include "GLFW/glfw3.h"
#include "glad/glad.h"

namespace Comet
{

	ImGuiLayer::~ImGuiLayer()
	{
	}

	void ImGuiLayer::onAttach()
	{
        ImGui::CreateContext();
		ImGui::StyleColorsDark();

		ImGuiIO& io = ImGui::GetIO();
		io.BackendFlags |= ImGuiBackendFlags_HasMouseCursors;
		io.BackendFlags |= ImGuiBackendFlags_HasSetMousePos;

        // Keyboard mapping. ImGui will use those indices to peek into the io.KeysDown[] array.
        //TODO: TEMP
        io.KeyMap[ImGuiKey_Tab] = GLFW_KEY_TAB;
        io.KeyMap[ImGuiKey_LeftArrow] = GLFW_KEY_LEFT;
        io.KeyMap[ImGuiKey_RightArrow] = GLFW_KEY_RIGHT;
        io.KeyMap[ImGuiKey_UpArrow] = GLFW_KEY_UP;
        io.KeyMap[ImGuiKey_DownArrow] = GLFW_KEY_DOWN;
        io.KeyMap[ImGuiKey_PageUp] = GLFW_KEY_PAGE_UP;
        io.KeyMap[ImGuiKey_PageDown] = GLFW_KEY_PAGE_DOWN;
        io.KeyMap[ImGuiKey_Home] = GLFW_KEY_HOME;
        io.KeyMap[ImGuiKey_End] = GLFW_KEY_END;
        io.KeyMap[ImGuiKey_Insert] = GLFW_KEY_INSERT;
        io.KeyMap[ImGuiKey_Delete] = GLFW_KEY_DELETE;
        io.KeyMap[ImGuiKey_Backspace] = GLFW_KEY_BACKSPACE;
        io.KeyMap[ImGuiKey_Space] = GLFW_KEY_SPACE;
        io.KeyMap[ImGuiKey_Enter] = GLFW_KEY_ENTER;
        io.KeyMap[ImGuiKey_Escape] = GLFW_KEY_ESCAPE;
        io.KeyMap[ImGuiKey_KeyPadEnter] = GLFW_KEY_KP_ENTER;
        io.KeyMap[ImGuiKey_A] = GLFW_KEY_A;
        io.KeyMap[ImGuiKey_C] = GLFW_KEY_C;
        io.KeyMap[ImGuiKey_V] = GLFW_KEY_V;
        io.KeyMap[ImGuiKey_X] = GLFW_KEY_X;
        io.KeyMap[ImGuiKey_Y] = GLFW_KEY_Y;
        io.KeyMap[ImGuiKey_Z] = GLFW_KEY_Z;

        ImGui_ImplOpenGL3_Init("#version 410");
	}

	void ImGuiLayer::onDetach()
	{
	}

	void ImGuiLayer::onUpdate()
	{
        ImGuiIO& io = ImGui::GetIO();
        Application& app = Application::get();
        io.DisplaySize = ImVec2(app.getWindow().getWidth(), app.getWindow().getHeight());

        ImGui_ImplOpenGL3_NewFrame();
        ImGui::NewFrame();

        float time = (float)glfwGetTime();
        io.DeltaTime = (m_time > 0.0f) ? (time - m_time) : (1.0f / 60.0f);
        m_time = time;

        static bool show = true;
        ImGui::ShowDemoWindow(&show);

        ImGui::Render();
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
	}

	void ImGuiLayer::onEvent(Event& e)
	{
        EventDispatcher dispatcher(e);

        dispatcher.dispatch<WindowResizedEvent>(CMT_BIND_EVENT_FUNCTION(ImGuiLayer::onWindowResizeEvent));

        dispatcher.dispatch<KeyPressedEvent>(CMT_BIND_EVENT_FUNCTION(ImGuiLayer::onKeyPressedEvent));
        dispatcher.dispatch<KeyReleasedEvent>(CMT_BIND_EVENT_FUNCTION(ImGuiLayer::onKeyReleasedEvent));
        dispatcher.dispatch<KeyTypedEvent>(CMT_BIND_EVENT_FUNCTION(ImGuiLayer::onKeyTypedEvent));

        dispatcher.dispatch<MouseButtonPressedEvent>(CMT_BIND_EVENT_FUNCTION(ImGuiLayer::onMouseButtonPressedEvent));
        dispatcher.dispatch<MouseButtonReleasedEvent>(CMT_BIND_EVENT_FUNCTION(ImGuiLayer::onMouseButtonReleasedEvent));
        dispatcher.dispatch<MouseMovedEvent>(CMT_BIND_EVENT_FUNCTION(ImGuiLayer::onMouseMovedEvent));
        dispatcher.dispatch<MouseScrolledEvent>(CMT_BIND_EVENT_FUNCTION(ImGuiLayer::onMouseScrolledEvent));
	}

    bool ImGuiLayer::onWindowResizeEvent(WindowResizedEvent& e)
    {
        ImGuiIO& io = ImGui::GetIO();
        io.DisplaySize = ImVec2(e.getWidth(), e.getHeight());
        io.DisplayFramebufferScale = ImVec2(1.0f, 1.0f);
        //TODO: TEMPORARY
        glViewport(0, 0, e.getWidth(), e.getHeight());

        return false;
    }

    bool ImGuiLayer::onKeyPressedEvent(KeyPressedEvent& e)
    {
        ImGuiIO& io = ImGui::GetIO();
        io.KeysDown[static_cast<int>(e.getKeyCode())] = true;

        io.KeyCtrl = io.KeysDown[GLFW_KEY_LEFT_CONTROL] | io.KeysDown[GLFW_KEY_RIGHT_CONTROL];
        io.KeyShift = io.KeysDown[GLFW_KEY_LEFT_SHIFT] | io.KeysDown[GLFW_KEY_RIGHT_SHIFT];
        io.KeyAlt = io.KeysDown[GLFW_KEY_LEFT_ALT] | io.KeysDown[GLFW_KEY_RIGHT_ALT];
        io.KeySuper = io.KeysDown[GLFW_KEY_LEFT_SUPER] | io.KeysDown[GLFW_KEY_RIGHT_SUPER];

        return io.WantCaptureKeyboard;
    }

    bool ImGuiLayer::onKeyReleasedEvent(KeyReleasedEvent& e)
    {
        ImGuiIO& io = ImGui::GetIO();
        io.KeysDown[static_cast<int>(e.getKeyCode())] = false;

        io.KeyCtrl = io.KeysDown[GLFW_KEY_LEFT_CONTROL] | io.KeysDown[GLFW_KEY_RIGHT_CONTROL];
        io.KeyShift = io.KeysDown[GLFW_KEY_LEFT_SHIFT] | io.KeysDown[GLFW_KEY_RIGHT_SHIFT];
        io.KeyAlt = io.KeysDown[GLFW_KEY_LEFT_ALT] | io.KeysDown[GLFW_KEY_RIGHT_ALT];
        io.KeySuper = io.KeysDown[GLFW_KEY_LEFT_SUPER] | io.KeysDown[GLFW_KEY_RIGHT_SUPER];

        return io.WantCaptureKeyboard;
    }

    bool ImGuiLayer::onKeyTypedEvent(KeyTypedEvent& e)
    {
        ImGuiIO& io = ImGui::GetIO();
        uint32_t keyNum = static_cast<uint32_t>(e.getKeyCode());
        if (keyNum > 0 && keyNum < 0x10000)
            io.AddInputCharacter(keyNum);

        return io.WantCaptureKeyboard;
    }

    bool ImGuiLayer::onMouseButtonPressedEvent(MouseButtonPressedEvent& e)
    {
        ImGuiIO& io = ImGui::GetIO();
        io.MouseDown[static_cast<int>(e.getKeyCode())] = true;

        return io.WantCaptureMouse;
    }

    bool ImGuiLayer::onMouseButtonReleasedEvent(MouseButtonReleasedEvent& e)
    {
        ImGuiIO& io = ImGui::GetIO();
        io.MouseDown[static_cast<int>(e.getKeyCode())] = false;

        return io.WantCaptureMouse;
    }

    bool ImGuiLayer::onMouseMovedEvent(MouseMovedEvent& e)
    {
        ImGuiIO& io = ImGui::GetIO();
        io.MousePos = ImVec2(e.getMousePosX(), e.getMousePosY());

        return io.WantCaptureMouse;
    }

    bool ImGuiLayer::onMouseScrolledEvent(MouseScrolledEvent& e)
    {
        ImGuiIO& io = ImGui::GetIO();
        io.MouseWheel += e.getYOffset();
        io.MouseWheelH += e.getXOffset();

        return io.WantCaptureMouse;
    }

}
