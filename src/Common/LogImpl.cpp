#include "LogImpl.hpp"

namespace Penrose {

    LogImpl::LogImpl(const ResourceSet *resources)
        : _resources(resources) {
        //
    }

    auto LogImpl::write(const LogLevel level, const std::string_view tag, const std::string_view msg) -> void {
        for (const auto &sink: this->_sinks) {
            sink->write(level, tag, msg);
        }
    }

    void LogImpl::addSink(std::type_index &&type) {
        this->_sinks.push_back(this->_resources->resolveOne<LogSink>(type));
    }
}
