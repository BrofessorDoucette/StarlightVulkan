#pragma once

#include <stdexcept>
#include <iostream>
#include "GLFW/glfw3.h"
#include <vector>

namespace Core {

    class Window {

    public:
        Window(const ApplicationInfo& t_app_info, int t_window_width, int t_window_height);

        ~Window();

        Window(const Window& window) = delete;
        Window& operator=(const Window& window) = delete;

        bool should_close_window();

        void poll_events();

        std::vector<const char*> get_required_extensions();


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
