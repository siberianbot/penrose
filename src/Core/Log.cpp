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

    void Log::writeDebug(std::string_view tag, std::string_view msg) {
        this->write(LogLevel::Debug, tag, msg);
    }

    void Log::writeInfo(std::string_view tag, std::string_view msg) {
        this->write(LogLevel::Info, tag, msg);
    }

    void Log::writeWarning(std::string_view tag, std::string_view msg) {
        this->write(LogLevel::Warning, tag, msg);
    }

    void Log::writeError(std::string_view tag, std::string_view msg) {
        this->write(LogLevel::Error, tag, msg);
    }

    void Log::write(LogLevel level, std::string_view tag, std::string_view msg) {
        std::cout << toString(level) << "/" << tag << ":\t" << msg << std::endl;
    }
}
