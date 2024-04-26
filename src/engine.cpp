#include "engine.hpp"
#include "instance.hpp"

namespace Core {

    Engine::Engine() {

        if (debugEnabled) {
            std::cout << "Constructing Graphics Engine! \n" << std::endl;
        }

        build_glfw_window();
        make_instance();
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

    Engine::~Engine() {

        if (debugEnabled) {
            std::cout << "Terminating GLFW Window!\n";
        }

        instance.destroy();

        //terminate glfw
        glfwTerminate();
    }

    void Engine::make_instance() {

        instance = vkInit::make_instance(debugEnabled,
                                         applicationName,
                                         applicationVersion,
                                         engineName,
                                         engineVersion);

    }

}