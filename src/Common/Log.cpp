#include <Penrose/Common/Log.hpp>

#include <Penrose/Resources/ResourceSet.hpp>

namespace Penrose {

    Log::Log(ResourceSet *resources)
            : _sinks(resources->getAllLazy<LogSink>()) {
        //
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
        for (const auto &sink: this->_sinks) {
            sink->write(level, tag, msg);
        }
    }
}
