#pragma once
#include "../core/external.hpp"

namespace vkInit {

    vk::Instance make_instance(bool debugEnabled,
                               const char* applicationName,
                               uint32_t applicationVersion,
                               const char* engineName,
                               uint32_t engineVersion,
                               const std::vector<const char*>& layers);

    bool extensions_and_layers_supported(const std::vector<const char*>& extensions,
                              const std::vector<const char*>& layers,
                              bool debugEnabled);

}