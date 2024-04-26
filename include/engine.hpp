#pragma once
#include "config.hpp"

namespace Core {

    class Engine {

    public:

        Engine();

        ~Engine();

    private:

        //Whether to print debug messages in functions
        bool debugEnabled = true;

        //GLFW window parameters
        int width{1080};
        int height{720};
        GLFWwindow* window{nullptr};

        const char* applicationName = static_cast<const char*>("Starlight (Vulkan)");
        const uint32_t applicationVersion = 1;
        const char* engineName = static_cast<const char*>("Starlight");
        const uint32_t engineVersion = 1;

        vk::Instance instance {nullptr};

        //GLFW setup
        void build_glfw_window();

        void make_instance();

    };
}
