#pragma once
#include <vulkan/vulkan.hpp>
#include <GLFW/glfw3.h>

#include "context.hpp"

namespace RenderingAPI {

    class VulkanContext : public Context {

    private:

        //Vulkan Instance and Dependencies
        vk::Instance _instance {nullptr};
        vk::DebugUtilsMessengerEXT _debugMessenger {nullptr};
        vk::DispatchLoaderDynamic _dldi;

    public:

        VulkanContext();
        ~VulkanContext() override;

        vk::Instance make_instance(bool debugEnabled,
                                   const char* applicationName,
                                   uint32_t applicationVersion,
                                   const char* engineName,
                                   uint32_t engineVersion,
                                   const std::vector<const char*>& layers);

        bool extensions_and_layers_supported(const std::vector<const char*>& extensions,
                                             const std::vector<const char*>& layers);



    };


}


