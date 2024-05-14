
#include "context.hpp"

namespace RenderingAPI {

    Context::Context(const Core::ApplicationInfo &t_app_info) : m_app_info(t_app_info) {

        create_vulkan_instance();

    };


    Context::~Context() {

    }

    void Context::create_vulkan_instance() {

        Core::Logger<Core::LogLevel::kInfo>::instance()
            << "Creating vulkan instance. \n";

        uint32_t version {0};
        vkEnumerateInstanceVersion(&version);

        Core::Logger<Core::LogLevel::kInfo>::instance()
            << "Began creating instance. \n"
            << "Vulkan Version: "
            << "Major: " << VK_API_VERSION_MAJOR(version)
            << ", Minor: " << VK_API_VERSION_MINOR(version)
            << ", Patch: " << VK_API_VERSION_PATCH(version)
            << ", Variant: " << VK_API_VERSION_VARIANT(version) << std::endl;

        version &= ~(0xFFFU);
        version = VK_MAKE_API_VERSION(0, 1, 0, 0);

        vk::ApplicationInfo app_info = vk::ApplicationInfo(m_app_info.kApplicationName,
                                                           VK_MAKE_VERSION(1, 0, 0),
                                                           m_app_info.kEngineName,
                                                           VK_MAKE_VERSION(1, 0, 0),
                                                           version,
                                                           nullptr);

        auto extensions =

        vk::InstanceCreateInfo instance_create_info = vk::InstanceCreateInfo(&app_info,
                                                                             );


    }

    void Context::set_required_extensions(const std::vector<const char *>&& extensions) {
        m_required_extensions = extensions;
    }


}