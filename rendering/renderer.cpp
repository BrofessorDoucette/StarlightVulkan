#include "rendering/renderer.hpp"

namespace Rendering {


    Renderer::Renderer(const char* t_application_name, const char* t_engine_name) :
        kApplicationName(t_application_name), kEngineName(t_engine_name){

    }

    Renderer::~Renderer() {

        Core::Logger<Core::LogLevel::kInfo>::instance()
            << " - Destructing Renderer Abstraction! \n";


    }

    bool Renderer::window_should_close() {
        return m_window.should_close_window();
    }

    void Renderer::poll_window_events() {
        m_window.poll_events();
    }

}