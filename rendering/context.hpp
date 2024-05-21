#pragma once

#include <iostream>
#include <vulkan/vulkan.hpp>

namespace Rendering {


    class Context {

    public:

        Context(const char* t_application_name, const char* t_engine_name, const std::vector<const char*>& t_required_extensions);
        ~Context() noexcept(false);

        Context(const Context& context) = delete;
        Context& operator=(const Context& context) = delete;

        vk::Instance instance() const; //Use only for temporary access to the vulkan instance within other rendering classes.

        [[nodiscard]] const std::vector<const char*>& required_extensions() const;
        [[nodiscard]] const std::vector<const char*>& required_layers() const;

    private:

        //Instance
        vk::Instance m_instance {nullptr};
        void create_vulkan_instance(const char* t_application_name,
                                    const char* t_engine_name,
                                    const std::vector<const char*>& t_required_extensions);

        bool extensions_and_layers_supported(const std::vector<const char *>& t_layers,
                                             const std::vector<const char *>& t_extensions);

        #if(BUILD_DEBUG)
            static constexpr bool kEnableVulkanDebug = true;
            const std::vector<const char*> m_required_debug_layers = {"VK_LAYER_KHRONOS_validation"};
            const std::vector<const char*> m_required_debug_extensions = {VK_EXT_DEBUG_UTILS_EXTENSION_NAME};
        #else
            static constexpr bool m_enable_validation_layers = false;
        const std::vector<const char*> m_requested_debug_layers = {};
            const std::vector<const char*> m_requested_debug_extensions = {};
        #endif

        std::vector<const char*> m_required_layers;
        std::vector<const char*> m_required_extensions;

        vk::DispatchLoaderDynamic m_dldi;
        void create_dynamic_dispatch_loader();

        static VKAPI_ATTR VkBool32 VKAPI_CALL debug_callback(
                VkDebugUtilsMessageSeverityFlagBitsEXT messageSeverity,
                VkDebugUtilsMessageTypeFlagsEXT messageType,
                const VkDebugUtilsMessengerCallbackDataEXT* pCallbackData,
                void* pUserData) {

            std::cerr << "validation layer: " << pCallbackData->pMessage << std::endl;

            return VK_FALSE;
        }

        vk::DebugUtilsMessengerEXT m_debug_messenger {nullptr};
        void create_debug_messenger();



    };
}
