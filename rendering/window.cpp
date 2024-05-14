#include "window.hpp"

namespace Core {

    Window::Window(const ApplicationInfo& t_app_info, int t_window_width, int t_window_height) :
                   m_window_name(t_app_info.kApplicationName),
                   m_window_width(t_window_width),
                   m_window_height(t_window_height){

        build_glfw_window();

    }

    Window::~Window() {

        glfwDestroyWindow(m_window);
        glfwTerminate();
    }

    void Window::build_glfw_window() {
        //Initialize glfw
        glfwInit();

        //No default rendering client, we'll hook vulkan up to the window later
        glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);

        //Resizing breaks the swapchain, we'll disable it for now
        glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);

        m_window = glfwCreateWindow(m_window_width, m_window_height, m_window_name, nullptr, nullptr);
        if(!m_window) {
            throw std::runtime_error("Failed to create GLFW window!");
        }

        //Core::DEBUG_MSG("Successfully made a GLFW window called: " << m_window_name);
        //std::cout << "Successfully made a GLFW window called: " << m_window_name << "\n";
        //std::cout << "Width: " << m_window_width << ", Height: " << m_window_height << "\n\n";

    }

    bool Window::should_close_window() {

        return glfwWindowShouldClose(m_window);
    }

    void Window::poll_events() {

        glfwPollEvents();

    }

    std::vector<const char *> Window::get_required_extensions() {

        uint32_t glfwExtensionCount = 0;
        const char** glfwExtensions = glfwGetRequiredInstanceExtensions(&glfwExtensionCount);
        std::vector<const char*> extensions(glfwExtensions, glfwExtensions + glfwExtensionCount);

        return extensions;

    }


}
