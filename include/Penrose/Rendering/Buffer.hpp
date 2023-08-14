#ifndef PENROSE_RENDERING_BUFFER_HPP
#define PENROSE_RENDERING_BUFFER_HPP

#include <cstdint>

namespace Penrose {

    enum class BufferType {
        Staging,
        Uniform,
        Vertex,
        Index
    };

    using BufferPtr = void *;

    class Buffer {
    public:
        virtual ~Buffer() = default;

        [[nodiscard]] virtual BufferType getType() const = 0;
        [[nodiscard]] virtual std::uint64_t getSize() const = 0;
        [[nodiscard]] virtual std::uint32_t getCount() const = 0;
        [[nodiscard]] virtual BufferPtr getPointer() const = 0;
    };
}

#endif // PENROSE_RENDERING_BUFFER_HPP
