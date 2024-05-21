
#include <set>

#include "core/logger.hpp"
#include "rendering/context.hpp"
#include "rendering/surface.hpp"
#include "rendering/device.hpp"

namespace Rendering {

    Device::Device(Context& t_context, Surface& t_surface) : m_context(t_context), m_surface(t_surface) {

        Core::Logger<Core::LogLevel::kInfo>::instance()
                << "\n" << "---Constructing Rendering Device!---" << "\n";

        pick_physical_device();
        create_logical_device();
        set_queue_handles();

    }

    Device::~Device() {

        Core::Logger<Core::LogLevel::kInfo>::instance()
                << " - Destructing Rendering Device Abstraction!\n";

        Core::Logger<Core::LogLevel::kInfo>::instance()
                << "\t - Destroying Vulkan Logical Device!\n";

        m_logical_device.destroy();
    }

    void Device::pick_physical_device() {

        Core::Logger<Core::LogLevel::kInfo>::instance()
            << "\t" << "- Picking physical device. \n";

        std::vector<vk::PhysicalDevice> devices = m_context.instance().enumeratePhysicalDevices();

        Core::Logger<Core::LogLevel::kInfo>::instance()
                << "\t" << "- Available Devices: \n";

        std::vector<int> scores (devices.size());

        int i = 0;
        for(const auto& device : devices) {
            Core::Logger<Core::LogLevel::kInfo>::instance()
                << "\t - " << i << ".) " << device.getProperties().deviceName << "\n";

            scores[i] = rate_device(device);
            i++;
        }

        auto highest_rated = std::distance(scores.begin(), std::max_element(scores.begin(), scores.end()));

        if (scores[highest_rated] < 0) {
            //This should be updated later if we need to support a variable number of queue families.
            throw std::runtime_error("No graphics device was found that supports all required queue families and extensions!");
        }

        m_queue_family_indices = find_queue_families(devices[highest_rated]);
        m_swap_chain_support_details = query_swap_chain_support(devices[highest_rated]);
        m_physical_device = devices[highest_rated];

        Core::Logger<Core::LogLevel::kInfo>::instance()
            << "\t- Picked highest rated device: " << devices[highest_rated].getProperties().deviceName << "\n"
            <<"\t- Queue Families: "
            << "Graphics: " << m_queue_family_indices.graphics_family.value() << ", "
            << "Present: " << m_queue_family_indices.present_family.value() << ".\n";

    }

    int Device::rate_device(vk::PhysicalDevice t_device) {

        QueueFamilyIndices indices = find_queue_families(t_device);

        if (!indices.is_complete() || !device_extensions_supported(t_device)) {

            return -1;
        }

        SwapChainSupportDetails details = query_swap_chain_support(t_device);
        if (details.formats.empty() && details.present_modes.empty()) {

            Core::Logger<Core::LogLevel::kError>::instance()
                << "\n" << "Swapchain extension was supported for this device, but no available formats or present modes are supported!\n";

            return -1;
        }

        auto properties = t_device.getProperties();
        auto features = t_device.getFeatures();

        int score = 0;

        switch(properties.deviceType) {

            case vk::PhysicalDeviceType::eDiscreteGpu:
                score += 1;
                break;
            default:
                break;
        }

        return score;
    }

    bool Device::device_extensions_supported(vk::PhysicalDevice t_device) {

        std::vector<vk::ExtensionProperties> supported_extensions = t_device.enumerateDeviceExtensionProperties();

        std::set<std::string> required_extensions {m_required_extensions.begin(), m_required_extensions.end()};

        for(const auto& extension: supported_extensions) {

            required_extensions.erase(extension.extensionName);
        }

        return required_extensions.empty();
    }

    QueueFamilyIndices Device::find_queue_families(vk::PhysicalDevice t_device) {
        QueueFamilyIndices indices;

        std::vector<vk::QueueFamilyProperties> queue_families = t_device.getQueueFamilyProperties();

        int i = 0;
        for(const auto& family : queue_families) {
            if((family.queueFlags & vk::QueueFlagBits::eGraphics) && !indices.graphics_family.has_value()) {
                indices.graphics_family = i;
            }

            if(t_device.getSurfaceSupportKHR(static_cast<uint32_t>(i), m_surface.surface()) && !indices.present_family.has_value()){
                indices.present_family = i;
            }

            if(indices.is_complete()){
                break;
            }
            i++;
        }

        return indices;
    }

    void Device::create_logical_device() {

        Core::Logger<Core::LogLevel::kInfo>::instance()
            << "\t- Creating logical device.... \n";

        float queue_priority = 1.0f;
        std::vector<vk::DeviceQueueCreateInfo> queue_create_infos;
        std::set<uint32_t> unique_queue_families {m_queue_family_indices.graphics_family.value(),
                                                  m_queue_family_indices.present_family.value()};

        for(auto family : unique_queue_families) {
            queue_create_infos.emplace_back(vk::DeviceQueueCreateFlags(),
                                        static_cast<uint32_t>(family),
                                        static_cast<uint32_t>(1),
                                        &queue_priority);
        }

        const std::vector<const char*>& required_layers = m_context.required_layers();

        vk::DeviceCreateInfo device_create_info = vk::DeviceCreateInfo(vk::DeviceCreateFlagBits(),
                                                                       static_cast<uint32_t>(queue_create_infos.size()),
                                                                       queue_create_infos.data(),
                                                                       static_cast<uint32_t>(required_layers.size()),
                                                                       required_layers.data(),
                                                                       static_cast<uint32_t>(m_required_extensions.size()),
                                                                       m_required_extensions.data());


        try {
            m_logical_device = m_physical_device.createDevice(device_create_info);

            Core::Logger<Core::LogLevel::kInfo>::instance()
                    << "\t- Successfully created logical device! \n";

        } catch (vk::Error& e) {
            throw std::runtime_error("Failed to create vulkan logical device!");
        }

    }

    void Device::set_queue_handles() {

        //Graphics Queue Handle
        m_graphics_queue = m_logical_device.getQueue(m_queue_family_indices.graphics_family.value(), 0);

        //Present Queue Handle
        m_present_queue = m_logical_device.getQueue(m_queue_family_indices.present_family.value(), 0);

        Core::Logger<Core::LogLevel::kInfo>::instance()
                << "\t- Retrieved queue handles from device and created queues!\n";
    }

    SwapChainSupportDetails Device::query_swap_chain_support(vk::PhysicalDevice t_device) {

        SwapChainSupportDetails details;

        details.capabilities = t_device.getSurfaceCapabilitiesKHR(m_surface.surface());
        details.formats = t_device.getSurfaceFormatsKHR(m_surface.surface());
        details.present_modes = t_device.getSurfacePresentModesKHR(m_surface.surface());

        return details;
    }

    const SwapChainSupportDetails& Device::swap_chain_support() const {
        return m_swap_chain_support_details;
    }

    const QueueFamilyIndices& Device::queue_family_indices() const {
        return m_queue_family_indices;
    }

    vk::Device Device::device() const {
        if(m_logical_device) {
            return m_logical_device;
        } else {
            throw std::runtime_error("Tried to access logical device but it evaluated to nullptr.");
        }
    }


}
