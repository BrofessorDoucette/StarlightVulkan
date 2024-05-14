#pragma once
#include <iostream>


namespace Core {

    enum class LogLevel {
        kInfo,
        kError,
    };

    template <typename T>
    concept Streamable = requires(std::ostream os, T value) {
        { os << value };
    };

    template <LogLevel level>
    class Logger {

    public:

        static Logger<level>& instance() {
            static Logger<level> INSTANCE {};
            return INSTANCE;
        };

        template <Streamable T>
        inline friend Logger& operator<<(Logger& logger, const T& t_msg){
            #if(BUILD_DEBUG)

                if constexpr (level == LogLevel::kInfo)
                    std::cout << t_msg;
                else if constexpr (level == LogLevel::kError)
                    std::cerr << t_msg;

            #endif

            return logger;
        };

        inline friend Logger& operator<<(Logger& logger, std::ostream& (*os)(std::ostream&)) {
            #if(BUILD_DEBUG)

                if constexpr (level == LogLevel::kInfo) {
                    std::cout << os;
                }
                else if constexpr (level == LogLevel::kError)
                    std::cerr << os;

            #endif

            return logger;
        };

        Logger(Logger const&) = delete;
        Logger& operator=(Logger const &) = delete;

    private:

        Logger() = default;
        ~Logger() = default;
    };

}

