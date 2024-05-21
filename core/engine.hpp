#pragma once

#include <iostream>
#include <stdexcept>

#include "core/logger.hpp"
#include "rendering/renderer.hpp"

namespace Core {

    class Engine {

    public:

        Engine();

        ~Engine();

        void run();

        Engine(const Engine& engine) = delete;
        Engine& operator=(const Engine& engine) = delete;

    private:

        //Member Variables
        //Application
        const char* kApplicationName = "Starlight (Vulkan)";
        const char* kEngineName = "Starlight";

        //Renderer
        Rendering::Renderer m_renderer {kApplicationName, kEngineName};

    };
}
