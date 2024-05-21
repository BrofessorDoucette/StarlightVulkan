
#include <set>
#include "core/logger.hpp"
#include "rendering/context.hpp"

namespace Rendering {

    Context::Context(const char* t_application_name,
                     const char* t_engine_name,
                     const std::vector<const char*>& t_required_extensions) {

        Core::Logger<Core::LogLevel::kInfo>::instance()
            << "\n" << "---Constructing Rendering Context!---" << "\n";

        create_vulkan_instance(t_application_name, t_engine_name, t_required_extensions);
        create_dynamic_dispatch_loader();
        create_debug_messenger();

    };


    Context::~Context() noexcept(false){

        Core::Logger<Core::LogLevel::kInfo>::instance()
                << " - Destructing Rendering Context Abstraction!\n"
                << "\t - Destroying Vulkan Debug Messenger! \n";

        if constexpr (kEnableVulkanDebug) {
            if (m_debug_messenger) {
                m_instance.destroyDebugUtilsMessengerEXT(m_debug_messenger, nullptr, m_dldi);
                m_debug_messenger = nullptr;
            }
        }

        Core::Logger<Core::LogLevel::kInfo>::instance()
                << "\t - Destroying Vulkan Instance!\n";

        try {
            m_instance.destroy();
            m_instance = nullptr;
        } catch (vk::Error& e) {
            throw std::runtime_error("Failed to destroy vulkan instance!");
        }

    }

    void Context::create_vulkan_instance(const char* t_application_name,
                                         const char* t_engine_name,
                                         const std::vector<const char*>& t_required_extensions) {

        uint32_t version {0};
        vkEnumerateInstanceVersion(&version);

        Core::Logger<Core::LogLevel::kInfo>::instance()
            << "\t"
            << "- Supported Vulkan Version: "
            << "Major: " << VK_API_VERSION_MAJOR(version)
            << ", Minor: " << VK_API_VERSION_MINOR(version)
            << ", Patch: " << VK_API_VERSION_PATCH(version)
            << ", Variant: " << VK_API_VERSION_VARIANT(version) << "\n";

        version &= ~(0xFFFU);
        version = VK_MAKE_API_VERSION(0, 1, 0, 0);

        Core::Logger<Core::LogLevel::kInfo>::instance()
            << "\t"
            << "- Defaulting to vulkan version 1.0.0 \n";


        vk::ApplicationInfo app_info = vk::ApplicationInfo(t_application_name,
                                                           VK_MAKE_VERSION(1, 0, 0),
                                                           t_engine_name,
                                                           VK_MAKE_VERSION(1, 0, 0),
                                                           version,
                                                           nullptr);

        std::vector<const char*> required_layers {};
        std::vector<const char*> required_extensions {t_required_extensions};

        if constexpr (kEnableVulkanDebug){
            for(const auto& layer : m_required_debug_layers) {
                required_layers.push_back(layer);
            }

            for(const auto& extension: m_required_debug_extensions) {
                required_extensions.push_back(extension);
            }
        }

        if(!extensions_and_layers_supported(required_layers, required_extensions)){
            throw std::runtime_error("Not all required extensions and layers are supported!");
        }

        vk::InstanceCreateInfo instance_create_info = vk::InstanceCreateInfo(vk::InstanceCreateFlags(),
                                                                             &app_info,
                                                                             static_cast<uint32_t>(required_layers.size()),
                                                                             required_layers.data(),
                                                                             static_cast<uint32_t>(required_extensions.size()),
                                                                             required_extensions.data(),
                                                                             nullptr);

        Core::Logger<Core::LogLevel::kInfo>::instance()
                << "\t" << "- Creating vulkan instance... \n";

        try {
            m_instance = vk::createInstance(instance_create_info);
            m_required_layers = std::move(required_layers);
            m_required_extensions = std::move(required_extensions);

            Core::Logger<Core::LogLevel::kInfo>::instance()
                    << "\t" << "- Successfully created vulkan instance! \n";
        } catch (vk::SystemError& err) {
            throw std::runtime_error("Failed to create vulkan instance!");
        }

    }

    bool Context::extensions_and_layers_supported(const std::vector<const char *>& t_layers,
                                                  const std::vector<const char *>& t_extensions) {

        std::vector<vk::LayerProperties> supported_layers = vk::enumerateInstanceLayerProperties();
        std::set<std::string> supported_layer_names {};

        Core::Logger<Core::LogLevel::kInfo>::instance()
            << "\t" << "- Instance can support the following layers:\n";

        for(auto& supported_layer : supported_layers) {

            Core::Logger<Core::LogLevel::kInfo>::instance() << "\t  - " << supported_layer.layerName << '\n';
            supported_layer_names.insert(supported_layer.layerName);
        }

        for(auto& layer: t_layers) {
            if(supported_layer_names.find(layer) == supported_layer_names.end()){

                Core::Logger<Core::LogLevel::kError>::instance() << "Layer: \"" << layer << "\" is not supported!\n";
                return false;
            }
        }

        std::vector<vk::ExtensionProperties> supported_extensions = vk::enumerateInstanceExtensionProperties();
        std::set<std::string> supported_extension_names {};

        Core::Logger<Core::LogLevel::kInfo>::instance()
                << "\t" << "- Instance can support the following extensions: \n";

        for (auto& supported_extension : supported_extensions) {

            Core::Logger<Core::LogLevel::kInfo>::instance() << "\t  - " << supported_extension.extensionName << "\n";
            supported_extension_names.insert(supported_extension.extensionName);
        }

        for (auto extension : t_extensions) {
            if(supported_extension_names.find(extension) == supported_extension_names.end()){

                Core::Logger<Core::LogLevel::kError>::instance() << "Extension: \"" << extension << "\" is not supported!\n";
                return false;
            }
        }

        return true;
    }

    void Context::create_dynamic_dispatch_loader() {

        try {
            m_dldi = vk::DispatchLoaderDynamic(m_instance, vkGetInstanceProcAddr);
        } catch (vk::Error& e) {
            throw std::runtime_error("Failed to create dynamic dispatch loader!");
        }
    }

    void Context::create_debug_messenger() {

        if constexpr (!kEnableVulkanDebug) {
            return;
        }

        vk::DebugUtilsMessengerCreateInfoEXT debug_create_info = vk::DebugUtilsMessengerCreateInfoEXT(
                        vk::DebugUtilsMessengerCreateFlagsEXT(),
                        (vk::DebugUtilsMessageSeverityFlagBitsEXT::eWarning | vk::DebugUtilsMessageSeverityFlagBitsEXT::eError),
                        (vk::DebugUtilsMessageTypeFlagBitsEXT::eGeneral | vk::DebugUtilsMessageTypeFlagBitsEXT::eValidation |
                        vk::DebugUtilsMessageTypeFlagBitsEXT::ePerformance | vk::DebugUtilsMessageTypeFlagBitsEXT::eDeviceAddressBinding),
                        debug_callback,
                        nullptr,
                        nullptr);

        try {
            m_debug_messenger = m_instance.createDebugUtilsMessengerEXT(debug_create_info, nullptr, m_dldi);
        } catch (vk::Error& e) {
            throw std::runtime_error("Failed to create debug messenger!");
        }
    }

    vk::Instance Context::instance() const {
        /*This function guarantees the vulkan instance exists only upon calling and does not guarantee it exists at any time later.*/
        if (m_instance) {
            return m_instance;
        } else {
            throw std::runtime_error("Tried to access instance but it doesn't exist!");
        }
    }

    const std::vector<const char *>& Context::required_layers() const{
        return m_required_layers;
    }

    const std::vector<const char *> &Context::required_extensions() const {
        return m_required_extensions;
    }

}