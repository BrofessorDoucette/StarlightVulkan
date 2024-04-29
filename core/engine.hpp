#pragma once
#include "external.hpp"

namespace Core {

    class Engine {

    public:

        Engine();

        ~Engine();

    private:

        //Whether to print debug messages in functions
        static constexpr bool debugEnabled = true;

        //GLFW window parameters
        int width{1080};
        int height{720};
        GLFWwindow* window{nullptr};

        const char* applicationName = static_cast<const char*>("Starlight (Vulkan)");
        const uint32_t applicationVersion = 1;
        const char* engineName = static_cast<const char*>("Starlight");
        const uint32_t engineVersion = 1;

        //Vulkan Instance and Dependencies
        vk::Instance instance {nullptr};
        vk::DebugUtilsMessengerEXT debugMessenger {nullptr};
        vk::DispatchLoaderDynamic dldi;
        std::vector<const char*> enabledLayers = determineLayers(debugEnabled);

        vk::PhysicalDevice physicalDevice {nullptr};
        vk::Device logicalDevice {nullptr};
        vk::Queue graphicsQueue {nullptr};

        static constexpr std::vector<const char*> determineLayers(bool debug) {

            std::vector<const char*> layers {};

            if(debug) {
                layers.push_back("VK_LAYER_KHRONOS_validation");
            }

            return layers;
        }

        //GLFW setup
        void build_glfw_window();

        void create_vulkan_instance();

        void choose_vulkan_physical_device();

        void create_vulkan_logical_device();

        void set_graphics_queue();

    };
}
