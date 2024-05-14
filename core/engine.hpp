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

        bool should_exit_safely();

        void poll_window_events();

        Engine(const Engine& engine) = delete;
        Engine& operator=(const Engine& engine) = delete;

    private:

        //Member Variables

        //Renderer
        RenderingAPI::Renderer m_renderer {kAppInfo, m_window};

        /*
        vk::SurfaceKHR surface {nullptr};

        std::vector<const char*> enabledLayers = determineLayers(_debugEnabled);

        vk::PhysicalDevice physicalDevice {nullptr};
        vk::Device logicalDevice {nullptr};
        vk::Queue graphicsQueue {nullptr};
        vk::Queue presentQueue {nullptr};

        static constexpr std::vector<const char*> determineLayers(bool debug) {

            std::vector<const char*> layers {};

            if(debug) {
                layers.push_back("VK_LAYER_KHRONOS_validation");
            }

            return layers;
        }

        void create_renderer(); */


    };
}
