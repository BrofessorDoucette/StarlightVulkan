#include "instance.hpp"

namespace vkInit {

    bool extensions_and_layers_supported(const std::vector<const char*>& extensions,
                                         const std::vector<const char*>& layers){

        std::vector<vk::ExtensionProperties> supportedExtensions = vk::enumerateInstanceExtensionProperties();
        std::set<std::string> supportedExtensionNames {};

        std::cout << "Instance can support the following extensions!" << std::endl;
        for (auto& supportedExtension : supportedExtensions) {

            std::cout << '\t' << supportedExtension.extensionName << "\n";
            supportedExtensionNames.insert(supportedExtension.extensionName);
        }

        for (auto extension : extensions) {
            if(supportedExtensionNames.find(extension) == supportedExtensionNames.end()){

                std::cout << "Extension: \"" << extension << "\" is not supported!" << std::endl;
                return false;
            }
        }

        std::vector<vk::LayerProperties> supportedLayers = vk::enumerateInstanceLayerProperties();
        std::set<std::string> supportedLayerNames {};

        std::cout << "Instance can support the following layers!" << std::endl;
        for(auto& supportedLayer : supportedLayers) {

            std::cout << '\t' << supportedLayer.layerName << '\n';
            supportedLayerNames.insert(supportedLayer.layerName);
        }

        for(auto& layer: layers) {
            if(supportedLayerNames.find(layer) == supportedLayerNames.end()){

                std::cout << '\t' << layer << "\" is not supported!" << std::endl;
                return false;
            }
        }

        return true;
    }

    vk::Instance make_instance(bool debugEnabled,
                               const char* applicationName,
                               uint32_t applicationVersion,
                               const char* engineName,
                               uint32_t engineVersion,
                               const std::vector<const char*>& layers) {


        uint32_t version {0};
        vkEnumerateInstanceVersion(&version);
        if (debugEnabled) {
            std::cout << "Began creating instance." << std::endl;
            std::cout << "Vulkan Version: "
                      << "Major: " << VK_API_VERSION_MAJOR(version)
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

            std::cout << "Debug Extensions Required:" << std::endl;
            std::cout << "\t\"" << "VK_EXT_debug_utils" << "\"" << std::endl;
            extensions.push_back("VK_EXT_debug_utils");
        }

        if (debugEnabled) {
            std::cout << "Layers requested:" << std::endl;
            for(auto layer : layers) {
                std::cout << "\t\"" << layer << "\"" << std::endl;
            }

            if(!extensions_and_layers_supported(extensions, layers)) {
                throw std::runtime_error("Vulkan extensions and layers specified are not supported!");
            }
            std::cout << "All instance layers and extensions requested are supported!" << std::endl;
        }

        vk::InstanceCreateInfo createInfo = vk::InstanceCreateInfo (
                vk::InstanceCreateFlags(),
                &appInfo,
                static_cast<uint32_t>(layers.size()),
                layers.data(),
                static_cast<uint32_t>(extensions.size()),
                extensions.data());

        try {

            return vk::createInstance(createInfo);

        } catch (vk::SystemError& err){

            if(debugEnabled) {
                std::cout << "Failed to create vulkan instance!" << std::endl;
            }

            throw std::runtime_error(err.what());
        }
    }

}