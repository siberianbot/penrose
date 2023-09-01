#ifndef PENROSE_RENDERING_BUFFER_FACTORY_HPP
#define PENROSE_RENDERING_BUFFER_FACTORY_HPP

#include <cstdint>

#include <Penrose/Rendering/Buffer.hpp>

namespace Penrose {

    class BufferFactory {
    public:
        virtual ~BufferFactory() = default;

        [[nodiscard]] virtual Buffer *makeBuffer(BufferType type, std::uint64_t size, std::uint32_t count,
                                                 bool map) = 0;
    };
}

#endif // PENROSE_RENDERING_BUFFER_FACTORY_HPP
