#pragma once

#include <memory>
#include <vector>
#include "context.hpp"
#include "window.hpp"

namespace RenderingAPI {

    class Renderer {

    public:

        explicit Renderer(const Core::ApplicationInfo& t_app_info, Core::Window& t_window);
        ~Renderer();

        Renderer(const Renderer& renderer) = delete;
        Renderer& operator=(const Renderer& renderer) = delete;

    private:

        //Window
        Core::Window& m_window;

        RenderingAPI::Context m_context;

    };


}