#pragma once
#include "../rendering/renderer.hpp"
#include <iostream>
#include <optional>
#include <set>
/*
* Vulkan separates the concept of physical and logical devices.
*
  A physical device usually represents a single complete implementation of Vulkan
  (excluding instance-level functionality) available to the host,
  of which there are a finite number.

  A logical device represents an instance of that implementation
  with its own state and resources independent of other logical devices.
*/

namespace vkInit {

    struct QueueFamilyIndices {
        std::optional<uint32_t> graphicsFamily;
        std::optional<uint32_t> presentFamily;

        bool isComplete() {
            return graphicsFamily.has_value() && presentFamily.has_value();
        }

    };

    vk::PhysicalDevice choose_physical_device(vk::Instance& instance, bool debugEnabled);

    void log_device_properties(const vk::PhysicalDeviceProperties& properties);

    bool extensions_supported(const vk::PhysicalDevice& device,
                              const std::vector<const char*>& requestedExtensions);

    QueueFamilyIndices findQueueFamilies(vk::PhysicalDevice& device, bool debugEnabled);

    vk::Device create_logical_device(vk::PhysicalDevice& physicalDevice,
                                     const std::vector<const char*>& layers,
                                     bool debugEnabled);

    vk::Queue get_graphics_queue(vk::PhysicalDevice& physicalDevice, vk::Device& logicalDevice, bool debugEnabled);

}