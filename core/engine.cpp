#include "engine.hpp"
#include "../rendering/instance.hpp"
#include "../rendering/logging.hpp"
#include "../rendering/device.hpp"

namespace Core {

    Engine::Engine() {

        if (debugEnabled) {
            std::cout << "Constructing Graphics Engine! \n" << std::endl;
        }

        build_glfw_window();
        create_vulkan_instance();
        choose_vulkan_physical_device();
        create_vulkan_logical_device();
        set_graphics_queue();

    }

    Engine::~Engine() {

        if (debugEnabled) {
            std::cout << "\n";
            std::cout << "Terminating GLFW Window!\n";
        }

        logicalDevice.destroy();

        if(debugEnabled) {
            instance.destroyDebugUtilsMessengerEXT(debugMessenger, nullptr, dldi);
        }
        instance.destroy();

        //terminate glfw
        glfwTerminate();
    }

    void Engine::build_glfw_window() {

        //Initialize glfw
        glfwInit();

        //No default rendering client, we'll hook vulkan up to the window later
        glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
        //Resizing breaks the swapchain, we'll disable it for now
        glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);

        window = glfwCreateWindow(width, height, applicationName, nullptr, nullptr);

        if(!window) {
            throw std::runtime_error("Failed to create GLFW window!");
        }


        if (debugEnabled) {
            std::cout << "Successfully made a GLFW window called: " << applicationName << std::endl;
            std::cout << "Width: " << width << ", Height: " << height << "\n" << std::endl;
        }
    }

    void Engine::create_vulkan_instance() {

        instance = vkInit::make_instance(debugEnabled,
                                         applicationName,
                                         applicationVersion,
                                         engineName,
                                         engineVersion,
                                         enabledLayers);

        dldi = vk::DispatchLoaderDynamic(instance, vkGetInstanceProcAddr);

        if(debugEnabled) {

            debugMessenger = vkInit::make_debug_messenger(instance, dldi);

        }

    }

    void Engine::choose_vulkan_physical_device() {

        physicalDevice = vkInit::choose_physical_device(instance, debugEnabled);

        if(!physicalDevice) {
            throw std::runtime_error("No physical device was suitable to be chosen!");
        }

        if(debugEnabled) {
            std::cout << "Picked device: " << std::endl;
            std::cout << (physicalDevice.getProperties()).deviceName << std::endl;
            std::cout << "\n";
        }

    }

    void Engine::create_vulkan_logical_device() {

        logicalDevice = vkInit::create_logical_device(physicalDevice, enabledLayers, debugEnabled);

    }

    void Engine::set_graphics_queue() {

        graphicsQueue = vkInit::get_graphics_queue(physicalDevice, logicalDevice, debugEnabled);

    }


}