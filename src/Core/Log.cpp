#include <Penrose/Core/Log.hpp>

#include <iostream>

#include <Penrose/Common/EngineError.hpp>

namespace Penrose {

    std::string_view toString(LogLevel level) {
        switch (level) {
            case LogLevel::Debug:
                return "Debug";

            case LogLevel::Info:
                return "Info";

            case LogLevel::Warning:
                return "Warning";

            case LogLevel::Error:
                return "Error";

            default:
                throw EngineError("Log level is not supported");
        }
    }

    void Log::writeDebug(std::string_view src, std::string_view msg) const {
        this->write(LogLevel::Debug, src, msg);
    }

    void Log::writeInfo(std::string_view src, std::string_view msg) const {
        this->write(LogLevel::Info, src, msg);
    }

    void Log::writeWarning(std::string_view src, std::string_view msg) const {
        this->write(LogLevel::Warning, src, msg);
    }

    void Log::writeError(std::string_view src, std::string_view msg) const {
        this->write(LogLevel::Error, src, msg);
    }

    void Log::write(LogLevel level, std::string_view src, std::string_view msg) const {
        std::cout << toString(level) << "/" << src << ":\t" << msg << std::endl;
    }
}
