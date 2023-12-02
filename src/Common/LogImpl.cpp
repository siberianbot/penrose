#include "LogImpl.hpp"

namespace Penrose {

    LogImpl::LogImpl(ResourceSet *resources)
        : _resources(resources),
          _sinks(resources->get<LogSink>()) {
        //
    }

    auto LogImpl::write(const LogLevel level, const std::string_view tag, const std::string_view msg) -> void {
        for (const auto &sink: this->_sinks) {
            sink->write(level, tag, msg);
        }
    }

    void LogImpl::addSink(std::type_index &&index) {
        // TODO: requires resource set reworking
    }
}
