#include "Utils.hpp"

#include <Penrose/Common/EngineError.hpp>

namespace Penrose {

    std::uint8_t getChannels(ImageFormat format) {
        switch (format) {
            case ImageFormat::RGBA:
                return 4;

            default:
                throw EngineError("Image format is not supported");
        }
    }
}
