#include "core/engine.hpp"

int main() {

    try {

        auto graphicsEngine = Core::Engine();

        while (!graphicsEngine.should_exit_safely()) {

            graphicsEngine.poll_window_events();

        }

    } catch (const std::exception& e) {
        std::cerr << e.what() << "\n";
        return EXIT_FAILURE;
    };

	return EXIT_SUCCESS;
}