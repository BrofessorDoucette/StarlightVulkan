#include "engine.hpp"


namespace Core {

    Engine::Engine(){

        Logger<LogLevel::kInfo>::instance()
            << "Starting Starlight Graphics Engine!\n";

    }

    Engine::~Engine() {

        Logger<LogLevel::kInfo>::instance()
            << "\n"
            << "Terminating GLFW Window!\n";
    }

    bool Engine::should_exit_safely() {

        return m_window.should_close_window();
    }

    void Engine::poll_window_events() {

        m_window.poll_events();
    }


    /*
    void Engine::create_rendering_context() {

        //context = RenderingAPI::Context(RenderingAPI::GraphicsAPI::Vulkan);

    }

    void Engine::create_vulkan_instance() {

        instance = vkInit::make_instance(debugEnabled,
                                         applicationName,
                                         applicationVersion,
                                         engineName,
                                         engineVersion,
                                         enabledLayers);

        dldi = vk::DispatchLoaderDynamic(instance, vkGetInstanceProcAddr);

        if(_debug == Debug::Enabled) {

            debugMessenger = vkInit::make_debug_messenger(instance, dldi);

        }

        VkSurfaceKHR surfaceTmp;
        if(glfwCreateWindowSurface(instance, window, nullptr, &surfaceTmp) != VK_SUCCESS){
            if(_debug == Debug::Enabled) {
                std::cout << "Failed to abstract the glfw surface for Vulkan. \n";
            }
        } else if (_debug == Debug::Enabled) {
            std::cout << "Successfully abstracted the glfw surface for Vulkan. \n";
        }

        surface = surfaceTmp;

    }

    void Engine::choose_vulkan_physical_device() {

        physicalDevice = vkInit::choose_physical_device(instance, debugEnabled);

        if(!physicalDevice) {
            throw std::runtime_error("No physical device was suitable to be chosen!");
        }

        if(_debug == Debug::Enabled) {
            std::cout << "Picked device: \n";
            std::cout << (physicalDevice.getProperties()).deviceName;
            std::cout << "\n";
        }

    }

    void Engine::create_vulkan_logical_device() {

        logicalDevice = vkInit::create_logical_device(physicalDevice, enabledLayers, debugEnabled);

    }

    void Engine::set_graphics_queue() {

        graphicsQueue = vkInit::get_graphics_queue(physicalDevice, logicalDevice, debugEnabled);

    }*/


}