#pragma once
#include "config.hpp"

namespace vkInit {

    vk::Instance make_instance(bool debugEnabled,
                               const char* applicationName,
                               uint32_t applicationVersion,
                               const char* engineName,
                               uint32_t engineVersion) {


        uint32_t version {0};
        vkEnumerateInstanceVersion(&version);
        if (debugEnabled) {

            std::cout << "Vulkan Version: "
            << ", Major: " << VK_API_VERSION_MAJOR(version)
            << ", Minor: " << VK_API_VERSION_MINOR(version)
            << ", Patch: " << VK_API_VERSION_PATCH(version)
            << ", Variant: " << VK_API_VERSION_VARIANT(version) << std::endl;

        }

        version &= ~(0xFFFU);
        version = VK_MAKE_API_VERSION(0, 1, 0, 0);

        vk::ApplicationInfo appInfo = vk::ApplicationInfo (
                applicationName,
                applicationVersion,
                engineName,
                engineVersion,
                version); //Some of these could be updated in the future!

        uint32_t glfwExtensionCount = 0;
        const char** glfwExtensions = glfwGetRequiredInstanceExtensions(&glfwExtensionCount);
        std::vector<const char*> extensions(glfwExtensions, glfwExtensions + glfwExtensionCount);
        if (debugEnabled) {

            std::cout << "GLFW Extensions Required:" << std::endl;

            for (const char* extensionName : extensions) {
                std::cout << "\t\"" << extensionName << "\"" << std::endl;
            }
        }

        std::vector<const char*> layers;

        vk::InstanceCreateInfo createInfo = vk::InstanceCreateInfo (
                vk::InstanceCreateFlags(),
                &appInfo,
                0,
                nullptr,
                static_cast<uint32_t>(extensions.size()),
                extensions.data());

        try {

            return vk::createInstance(createInfo);

        } catch (vk::SystemError& err) {

            if(debugEnabled) {
                std::cout << "Failed to create vulkan instance!" << std::endl;
            }

            throw std::runtime_error(err.what());
        }
    }

}