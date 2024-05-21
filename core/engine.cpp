#include "engine.hpp"

namespace Core {

    Engine::Engine(){

    }

    Engine::~Engine() {

        Logger<LogLevel::kInfo>::instance()
            << "\n"
            << "---Began Terminating!---\n"
            << " - Destructing Engine! \n";
    }

    void Engine::run() {

        Logger<LogLevel::kInfo>::instance()
                << "\n" << "---Engine Construction Finished!---\n"
                << "\n" << "Running main loop!\n";

        while(!m_renderer.window_should_close()) {
            m_renderer.poll_window_events();
        }

    }

}