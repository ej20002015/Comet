#include "CometPCH.h"
#include "Comet/Core/Input.h"

#include "Comet/Core/Application.h"

#include "GLFW/glfw3.h"

namespace Comet
{

    bool Input::isKeyPressed(KeyCode keyCode)
    {
        GLFWwindow* window = static_cast<GLFWwindow*>(Application::get().getWindow().getImplementationWindow());
        int action = glfwGetKey(window, static_cast<int>(keyCode));
        return action == GLFW_PRESS;
    }

    bool Input::isMouseButtonPressed(MouseButtonCode mouseCode)
    {
        GLFWwindow* window = static_cast<GLFWwindow*>(Application::get().getWindow().getImplementationWindow());
        int action = glfwGetMouseButton(window, static_cast<int>(mouseCode));
        return action == GLFW_PRESS;
    }

    glm::vec2 Input::getMousePosition()
    {
        GLFWwindow* window = static_cast<GLFWwindow*>(Application::get().getWindow().getImplementationWindow());
        double xPos, yPos;
        glfwGetCursorPos(window, &xPos, &yPos);

        return { static_cast<float>(xPos), static_cast<float>(yPos) };
    }

}
