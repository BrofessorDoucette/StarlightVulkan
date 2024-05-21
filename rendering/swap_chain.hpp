#pragma once

#include "rendering/device.hpp"
#include "rendering/surface.hpp"

namespace Rendering {

    class SwapChain {

    public:

        SwapChain(Device& t_device, Window& t_window, Surface& t_surface);
        ~SwapChain();

        SwapChain(const SwapChain& swap_chain) = delete;
        SwapChain& operator=(const SwapChain& swap_chain) = delete;

    private:

        //Dependencies
        Device& m_device;
        Window& m_window;
        Surface& m_surface;

        //Swap Chain
        vk::SwapchainKHR m_swap_chain;
        void create_swap_chain();

        //Surface Format
        vk::SurfaceFormatKHR m_surface_format;
        vk::SurfaceFormatKHR choose_swap_surface_format(const std::vector<vk::SurfaceFormatKHR>& t_available_formats);

        vk::PresentModeKHR m_present_mode;
        vk::PresentModeKHR choose_surface_present_mode(const std::vector<vk::PresentModeKHR>& t_available_present_modes);

        vk::Extent2D m_extent;
        vk::Extent2D  choose_swap_extent(const vk::SurfaceCapabilitiesKHR& t_capabilities);

        //Images
        std::vector<vk::Image> m_images;

    };
}


