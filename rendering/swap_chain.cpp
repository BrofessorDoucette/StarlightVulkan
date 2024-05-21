#include "rendering/swap_chain.hpp"

namespace Rendering {


    SwapChain::SwapChain(Device& t_device, Window& t_window, Surface& t_surface) :
        m_device(t_device), m_window(t_window), m_surface(t_surface) {

        Core::Logger<Core::LogLevel::kInfo>::instance()
                << "\n" << "---Constructing Rendering Swap Chain!---" << "\n";

        create_swap_chain();

    }

    SwapChain::~SwapChain() {

        Core::Logger<Core::LogLevel::kInfo>::instance()
                << " - Destructing Rendering Swap Chain Abstraction! \n"
                << "\t - Destroying Vulkan Swap Chain KHR! \n";

        m_device.device().destroySwapchainKHR(m_swap_chain);

    }

    void SwapChain::create_swap_chain() {

        const SwapChainSupportDetails& support = m_device.swap_chain_support();

        vk::SurfaceFormatKHR surface_format = choose_swap_surface_format(support.formats);
        vk::PresentModeKHR present_mode = choose_surface_present_mode(support.present_modes);
        vk::Extent2D extent = choose_swap_extent(support.capabilities);

        uint32_t image_count = support.capabilities.minImageCount + 1;

        if (support.capabilities.maxImageCount < image_count) {
            throw std::runtime_error("Tried to create a swap chain but the maxImageCount supported is less than the required image count!");
        }

        const QueueFamilyIndices& indices = m_device.queue_family_indices();
        uint32_t queue_family_indices[] = {indices.graphics_family.value(), indices.present_family.value()};

        vk::SharingMode sharing_mode;
        uint32_t queue_family_index_count;
        uint32_t* p_queue_family_indices;

        if(indices.graphics_family == indices.present_family){
            sharing_mode = vk::SharingMode::eExclusive;
            queue_family_index_count = static_cast<uint32_t>(0);
            p_queue_family_indices = nullptr;
        } else {
            sharing_mode = vk::SharingMode::eConcurrent;
            queue_family_index_count = static_cast<uint32_t>(2);
            p_queue_family_indices = queue_family_indices;
        }

        vk::SwapchainCreateInfoKHR create_info = vk::SwapchainCreateInfoKHR(vk::SwapchainCreateFlagsKHR(),
                                                                            m_surface.surface(),
                                                                            image_count,
                                                                            surface_format.format,
                                                                            surface_format.colorSpace,
                                                                            extent,
                                                                            static_cast<uint32_t>(1),
                                                                            vk::ImageUsageFlagBits::eColorAttachment,
                                                                            sharing_mode,
                                                                            queue_family_index_count,
                                                                            p_queue_family_indices,
                                                                            support.capabilities.currentTransform,
                                                                            vk::CompositeAlphaFlagBitsKHR::eOpaque,
                                                                            present_mode,
                                                                            vk::True,
                                                                            VK_NULL_HANDLE);

        try {
            m_swap_chain = m_device.device().createSwapchainKHR(create_info);

            Core::Logger<Core::LogLevel::kInfo>::instance()
                    << "\t- " << "Successfully created vulkan swap chain! \n";

            m_surface_format = surface_format;
            m_present_mode = present_mode;
            m_extent = extent;

            m_images = m_device.device().getSwapchainImagesKHR(m_swap_chain);


        } catch (vk::Error& e) {
            throw std::runtime_error("Failed to create vulkan swap chain!");
        }

    }

    vk::SurfaceFormatKHR SwapChain::choose_swap_surface_format(const std::vector<vk::SurfaceFormatKHR>& t_available_formats) {

        Core::Logger<Core::LogLevel::kInfo>::instance()
                << "\t- Choosing surface format! \n";

        for (const auto& surface_format : t_available_formats) {
            if (surface_format.format == vk::Format::eB8G8R8A8Srgb && surface_format.colorSpace == vk::ColorSpaceKHR::eSrgbNonlinear) {
                return surface_format;
            }
        }

        Core::Logger<Core::LogLevel::kError>::instance()
            << "Swap chain format with VK_FORMAT_B8G8R8A8_SRGB and VK_COLOR_SPACE_SRGB_NONLINEAR_KHR unavailable! \n"
            << "Settling for first available format! \n";

        return t_available_formats[0];
    }

    vk::PresentModeKHR
    SwapChain::choose_surface_present_mode(const std::vector<vk::PresentModeKHR>& t_available_present_modes) {

        Core::Logger<Core::LogLevel::kInfo>::instance()
                << "\t- Choosing surface present mode! \n";

        for (const auto& present_mode : t_available_present_modes) {
            if (present_mode == vk::PresentModeKHR::eMailbox) {
                return present_mode;
            }
        }

        Core::Logger<Core::LogLevel::kError>::instance()
            << "Mailbox present mode was unavailable, defaulting to FIFO. \n";

        return vk::PresentModeKHR::eFifo; //FIFO is guaranteed to be available
    }

    vk::Extent2D SwapChain::choose_swap_extent(const vk::SurfaceCapabilitiesKHR& t_capabilities) {

        Core::Logger<Core::LogLevel::kInfo>::instance()
                << "\t- Calculating surface extent! \n";

        if(t_capabilities.currentExtent.width != std::numeric_limits<uint32_t>::max()) {
            return t_capabilities.currentExtent;
        } else {

            int width, height;
            glfwGetFramebufferSize(m_window.glfw_window(), &width, &height);

            vk::Extent2D extent {static_cast<uint32_t>(width), static_cast<uint32_t>(height)};


            extent.width = std::clamp(extent.width, t_capabilities.minImageExtent.width, t_capabilities.maxImageExtent.width);
            extent.height = std::clamp(extent.height, t_capabilities.minImageExtent.height, t_capabilities.maxImageExtent.height);

            return extent;

        }

    }


}