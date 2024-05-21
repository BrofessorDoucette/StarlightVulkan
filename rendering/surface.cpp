
#include "rendering/surface.hpp"

namespace Rendering {

    Surface::Surface(Window& t_window, Context& t_context) : m_window(t_window), m_context(t_context) {

        Core::Logger<Core::LogLevel::kInfo>::instance()
                << "\n" << "---Constructing Rendering Surface!---" << "\n";

        create_surface();

    }

    Surface::~Surface() {

        Core::Logger<Core::LogLevel::kInfo>::instance()
                << " - Destructing Rendering Surface Abstraction! \n";

        Core::Logger<Core::LogLevel::kInfo>::instance()
            << "\t - Destroying Vulkan Surface KHR! \n";

        m_context.instance().destroySurfaceKHR(m_surface);

    }



    void Surface::create_surface() {

        VkSurfaceKHR surface;
        if(glfwCreateWindowSurface(m_context.instance(), m_window.glfw_window(), nullptr, &surface) != VK_SUCCESS){

            throw std::runtime_error("Failed to create vulkan window surface!");

        }

        m_surface = surface;

        Core::Logger<Core::LogLevel::kInfo>::instance()
            << "\t- " << "Successfully created vulkan surface! \n";

    }

    vk::SurfaceKHR Surface::surface() const{
        if(m_surface) {
            return m_surface;
        } else {
            throw std::runtime_error("Tried to access surface but it doesn't exist!");
        }
    }


}