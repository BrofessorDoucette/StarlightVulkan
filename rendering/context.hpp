#pragma once

#include <vulkan/vulkan.hpp>

#include "core/logger.hpp"

namespace RenderingAPI {


    class Context {

    public:

        Context(const Core::ApplicationInfo& t_app_info, const Core::Window& t_window);
        ~Context();

    private:

        //Application
        Core::ApplicationInfo m_app_info;

        //GLFW
        std::vector<const char*> m_required_extensions;

        //Instance
        vk::Instance m_instance;
        void create_vulkan_instance();

    };
}
