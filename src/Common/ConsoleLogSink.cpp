#include "ConsoleLogSink.hpp"

#include <iostream>

#include "src/Utils/StringifyUtils.hpp"

namespace Penrose {

    void ConsoleLogSink::write(const LogLevel level, const std::string_view tag, const std::string_view msg) {
        std::cout << toString(level) << "/" << tag << ":\t" << msg << std::endl;
    }
}
