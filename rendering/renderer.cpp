#include "renderer.hpp"

namespace RenderingAPI {


    Renderer::Renderer(const Core::ApplicationInfo& t_app_info, Core::Window& t_window) :
        m_app_info(t_app_info), m_context(t_app_info), m_window(t_window) {

    }

    Renderer::~Renderer() {



    }

}