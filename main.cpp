#include "core/engine.hpp"

int main() {

    try {

        Core::Logger<Core::LogLevel::kInfo>::instance()
                << "----------------------------\n"
                << "---Creating Starlight Graphics Engine!---\n"
                << "\n";

        auto engine = Core::Engine();

        engine.run();

    } catch (const std::exception& e) {
        std::cerr << e.what() << "\n";
        return EXIT_FAILURE;
    };

	return EXIT_SUCCESS;
}