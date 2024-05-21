#pragma once

#include <memory>
#include <vector>

#include "rendering/window.hpp"
#include "rendering/context.hpp"
#include "rendering/surface.hpp"
#include "rendering/device.hpp"
#include "rendering/swap_chain.hpp"

namespace Rendering {

    class Renderer {

    public:

        explicit Renderer(const char* t_application_name, const char* t_engine_name);
        ~Renderer();

        Renderer(const Renderer& renderer) = delete;
        Renderer& operator=(const Renderer& renderer) = delete;

        bool window_should_close();

        void poll_window_events();

    private:

        //Application
        const char* kApplicationName;
        const char* kEngineName;

        //Window
        Window m_window {kApplicationName};

        //Context
        Context m_context {kApplicationName, kEngineName, m_window.get_required_extensions()};

        //Surface
        Surface m_surface {m_window, m_context};

        //Device
        Device m_device {m_context, m_surface};

        SwapChain m_swap_chain {m_device, m_window, m_surface};


    };


}