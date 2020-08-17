#include "CometPCH.h"
#include "WindowsInput.h"

#include "Comet/Core/Application.h"

#include "GLFW/glfw3.h"

namespace Comet
{

    bool WindowsInput::i_isKeyPressed(KeyCode keyCode) const
    {
        GLFWwindow* window = static_cast<GLFWwindow*>(Application::get().getWindow().getImplementationWindow());
        int action = glfwGetKey(window, static_cast<int>(keyCode));
        return action == GLFW_PRESS;
    }

    bool WindowsInput::i_isMouseButtonPressed(KeyCode keyCode) const
    {
        GLFWwindow* window = static_cast<GLFWwindow*>(Application::get().getWindow().getImplementationWindow());
        int action = glfwGetMouseButton(window, static_cast<int>(keyCode));
        return action == GLFW_PRESS;
    }

    std::pair<float, float> WindowsInput::i_getMousePosition() const
    {
        GLFWwindow* window = static_cast<GLFWwindow*>(Application::get().getWindow().getImplementationWindow());
        double xPos, yPos;
        glfwGetCursorPos(window, &xPos, &yPos);

        return { static_cast<float>(xPos), static_cast<float>(yPos) };
    }

}
