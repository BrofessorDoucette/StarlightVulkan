#include "device.hpp"

namespace vkInit {

    vk::PhysicalDevice choose_physical_device(vk::Instance& instance, bool debugEnabled) {

        if(debugEnabled) {
            std::cout << "\n";
            std::cout << "Choosing physical device.." << std::endl;

        }

        std::vector<vk::PhysicalDevice> availableDevices = instance.enumeratePhysicalDevices();

        if(availableDevices.empty()) {

            throw std::runtime_error("No physical graphics devices available!");

        } else {

            if(debugEnabled) {
                std::cout << "There are: " << availableDevices.size() << " physical devices available:\n" << std::endl;
            }
        }

        const std::vector<const char*> requestedExtensions {VK_KHR_SWAPCHAIN_EXTENSION_NAME};

        if(debugEnabled) {
            std::cout << "Requested physical device extensions: " << std::endl;
            for(auto extension : requestedExtensions) {
                std::cout << "\t\"" << extension << "\"" << std::endl;
            }
            std::cout << "\n";
        }

        vk::PhysicalDevice selectedDevice {nullptr};

        //This loop picks the first supported discrete GPU if it exists.
        //Else it just picks the first supported GPU.
        for(auto& device : availableDevices) {

            vk::PhysicalDeviceProperties properties = device.getProperties();

            if(debugEnabled){
                log_device_properties(properties);
            }

            if(extensions_supported(device, requestedExtensions)) {

                if(debugEnabled) {
                    std::cout << "This device supports all the required device extensions.\n" << std::endl;
                }

                if(properties.deviceType == vk::PhysicalDeviceType::eDiscreteGpu) {
                    selectedDevice = device;
                    break;
                } else {
                    if(!selectedDevice) {
                        selectedDevice = device;
                        continue;
                    }
                }
            } else {
                std::cout << "This device does not support all required extensions." << std::endl;
            }

        }

        return selectedDevice;

    }

    void log_device_properties(const vk::PhysicalDeviceProperties& properties) {

        /*
        * typedef struct VkPhysicalDeviceProperties {
            uint32_t                            apiVersion;
            uint32_t                            driverVersion;
            uint32_t                            vendorID;
            uint32_t                            deviceID;
            VkPhysicalDeviceType                deviceType;
            char                                deviceName[VK_MAX_PHYSICAL_DEVICE_NAME_SIZE];
            uint8_t                             pipelineCacheUUID[VK_UUID_SIZE];
            VkPhysicalDeviceLimits              limits;
            VkPhysicalDeviceSparseProperties    sparseProperties;
            } VkPhysicalDeviceProperties;
        */

        std::cout << "Device name: " << properties.deviceName << std::endl;
        std::cout << "Device type: ";
        switch (properties.deviceType) {

            case (vk::PhysicalDeviceType::eCpu):
                std::cout << "eCPU" << std::endl;
                break;
            case (vk::PhysicalDeviceType::eDiscreteGpu):
                std::cout << "Discrete GPU" << std::endl;
                break;
            case(vk::PhysicalDeviceType::eIntegratedGpu):
                std::cout << "Integrated GPU" << std::endl;
                break;
            case(vk::PhysicalDeviceType::eVirtualGpu):
                std::cout << "Virtual GPU" << std::endl;
                break;
            default:
                std::cout << "Unknown" << std::endl;

        }
    }

    bool extensions_supported(const vk::PhysicalDevice &device,
                              const std::vector<const char*>& requestedExtensions) {

        std::set<std::string> requiredExtensions(requestedExtensions.begin(), requestedExtensions.end());

        for(auto& extension : device.enumerateDeviceExtensionProperties()) {

            requiredExtensions.erase(extension.extensionName);

        }

        return requiredExtensions.empty();
    }

    QueueFamilyIndices findQueueFamilies(vk::PhysicalDevice& device, bool debugEnabled) {
        QueueFamilyIndices indices {};

        std::vector<vk::QueueFamilyProperties> queueFamilies = device.getQueueFamilyProperties();

        if(debugEnabled) {
            std::cout << "System can support a maximum of: " << queueFamilies.size() << " queue families." << std::endl;
        }

        int i = 0;
        for(auto& queueFamily : queueFamilies) {

            if(queueFamily.queueFlags & vk::QueueFlagBits::eGraphics) {
                indices.graphicsFamily = i;
                indices.presentFamily = i; //graphics family queues can likely present also

                if(debugEnabled) {
                    std::cout << "Queue family: " << i << ", is suitable for graphics and presenting." << std::endl;
                }
            }

            if(indices.isComplete()) {
                break;
            }

            i++;
        }

        if(!indices.isComplete()) {
            throw std::runtime_error("Queue family indices were never given valid values!");
        }

        return indices;
    }

    vk::Device create_logical_device(vk::PhysicalDevice& physicalDevice,
                                     const std::vector<const char*>& layers,
                                     bool debugEnabled) {

        QueueFamilyIndices indices = findQueueFamilies(physicalDevice, debugEnabled);

        float queuePriority = 1.0f;
        vk::DeviceQueueCreateInfo queueCreateInfo = vk::DeviceQueueCreateInfo(vk::DeviceQueueCreateFlags(),
                                                                              static_cast<uint32_t>(indices.graphicsFamily.value()),
                                                                              static_cast<uint32_t>(1),
                                                                              &queuePriority,
                                                                              nullptr);

        vk::PhysicalDeviceFeatures deviceFeatures = vk::PhysicalDeviceFeatures(); //Default Setup

        vk::DeviceCreateInfo deviceCreateInfo = vk::DeviceCreateInfo(vk::DeviceCreateFlags(),
                                                                     static_cast<uint32_t>(1),
                                                                     &queueCreateInfo,
                                                                     layers.size(),
                                                                     layers.data(),
                                                                     static_cast<uint32_t>(0),
                                                                     nullptr);
        try {
            vk::Device device = physicalDevice.createDevice(deviceCreateInfo);
            if(debugEnabled) {
                std::cout << "\n";
                std::cout << "GPU has been successfully abstracted into logical device!" << std::endl;
            }
            return device;
        } catch (vk::SystemError& err) {

            throw std::runtime_error("Unable to abstract physical GPU into vulkan logical device!");
        }
    }

    vk::Queue get_graphics_queue(vk::PhysicalDevice& physicalDevice, vk::Device& logicalDevice, bool debugEnabled) {

        QueueFamilyIndices indices = findQueueFamilies(physicalDevice, debugEnabled);

        return logicalDevice.getQueue(indices.graphicsFamily.value(), 0);
    }
}