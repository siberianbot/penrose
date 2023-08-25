#ifndef PENROSE_RENDERING_DESCRIPTOR_HPP
#define PENROSE_RENDERING_DESCRIPTOR_HPP

#include <unordered_set>

#include <Penrose/Rendering/DescriptorBindingValue.hpp>

namespace Penrose {

    class Descriptor {
    public:
        virtual ~Descriptor() = default;

        [[nodiscard]] virtual const std::unordered_set<DescriptorBindingValue> &getBindingValues() const = 0;
    };
}

#endif // PENROSE_RENDERING_DESCRIPTOR_HPP
