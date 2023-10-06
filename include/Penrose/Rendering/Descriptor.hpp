#ifndef PENROSE_RENDERING_DESCRIPTOR_HPP
#define PENROSE_RENDERING_DESCRIPTOR_HPP

#include <unordered_set>

#include <Penrose/Rendering/DescriptorBindingValue.hpp>

namespace Penrose {

    class Descriptor {
    public:
        virtual ~Descriptor() = default;

        virtual void updateBindingValues(const std::unordered_set<DescriptorBindingValue> &values) = 0;

        [[nodiscard]] virtual const std::unordered_set<DescriptorBindingValue> &getBindingValues() const = 0;
    };
}

#endif // PENROSE_RENDERING_DESCRIPTOR_HPP
