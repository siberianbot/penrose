#include <Penrose/Common/StdOutLogSink.hpp>

#include <iostream>

namespace Penrose {

    void StdOutLogSink::write(LogLevel level, std::string_view tag, std::string_view msg) {
        std::cout << toString(level) << "/" << tag << ":\t" << msg << std::endl;
    }
}
