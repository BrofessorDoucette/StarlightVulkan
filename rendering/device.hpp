#pragma once

#include <optional>

#include "rendering/context.hpp"
#include "rendering/surface.hpp"

namespace Rendering {

    struct SwapChainSupportDetails {

        vk::SurfaceCapabilitiesKHR capabilities;
        std::vector<vk::SurfaceFormatKHR> formats;
        std::vector<vk::PresentModeKHR> present_modes;

    };

    struct QueueFamilyIndices {
        std::optional<uint32_t> graphics_family;
        std::optional<uint32_t> present_family;


        bool is_complete() {
            return graphics_family.has_value() && present_family.has_value();
        }
    };

    class Device {

    public:

        Device(Context& t_context, Surface& t_surface);
        ~Device();

        Device(const Device& device) = delete;
        Device& operator=(const Device& device) = delete;

        const SwapChainSupportDetails& swap_chain_support() const;
        const QueueFamilyIndices& queue_family_indices() const;
        vk::Device device() const;

    private:

        //Dependencies
        Context& m_context;
        Surface& m_surface;

        //Physical Device
        vk::PhysicalDevice m_physical_device {nullptr};

        void pick_physical_device();
        int rate_device(vk::PhysicalDevice t_device);
        bool device_extensions_supported(vk::PhysicalDevice t_device);

        const std::vector<const char*> m_required_extensions = {VK_KHR_SWAPCHAIN_EXTENSION_NAME};

        //Queue Families
        QueueFamilyIndices m_queue_family_indices;
        QueueFamilyIndices find_queue_families(vk::PhysicalDevice t_device);

        //Logical Device
        vk::Device m_logical_device {nullptr};
        void create_logical_device();

        //Queues
        vk::Queue m_graphics_queue {nullptr};
        vk::Queue m_present_queue {nullptr};
        void set_queue_handles();

        SwapChainSupportDetails m_swap_chain_support_details;
        SwapChainSupportDetails query_swap_chain_support(vk::PhysicalDevice t_device);


    };
}