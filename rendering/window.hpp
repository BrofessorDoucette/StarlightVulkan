#pragma once

#include <vector>

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

namespace Rendering {

    class Window {

    public:
        Window(const char* t_window_name = "Starlight", int t_window_width = 1080, int t_window_height = 720);

        ~Window();

        Window(const Window& window) = delete;
        Window& operator=(const Window& window) = delete;

        bool should_close_window();

        void poll_events();

        [[nodiscard]] std::vector<const char*> get_required_extensions() const;

        GLFWwindow* glfw_window() const;

    private:

        //---------- Runtime Options -------------

        //Window
        const char* m_window_name;

        //----------- External ----------
        //GLFW
        int m_window_width;
        int m_window_height;
        GLFWwindow* m_window {nullptr};

        void build_glfw_window();

    };
}
