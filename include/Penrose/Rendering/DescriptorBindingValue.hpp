#ifndef PENROSE_RENDERING_DESCRIPTOR_BINDING_VALUE_HPP
#define PENROSE_RENDERING_DESCRIPTOR_BINDING_VALUE_HPP

#include <cstdint>
#include <optional>
#include <vector>

#include <Penrose/Utils/HashUtils.hpp>

namespace Penrose {

    class Buffer;
    class Image;
    class Sampler;

    class DescriptorBindingValue {
    public:
        explicit constexpr DescriptorBindingValue(std::uint32_t bindingIdx,
                                                  std::vector<Buffer *> &&buffers = {},
                                                  std::vector<Image *> &&images = {},
                                                  std::vector<Sampler *> &&samplers = {})
                : _bindingIdx(bindingIdx),
                  _buffers(buffers),
                  _images(images),
                  _samplers(samplers) {
            //
        }

        [[nodiscard]] constexpr DescriptorBindingValue &setBuffers(std::vector<Buffer *> &&buffers) {
            this->_buffers = buffers;

            return *this;
        }

        [[nodiscard]] constexpr DescriptorBindingValue &setBuffers(const std::vector<Buffer *> &buffers) {
            this->_buffers = buffers;

            return *this;
        }

        [[nodiscard]] constexpr DescriptorBindingValue &setImages(std::vector<Image *> &&images) {
            this->_images = images;

            return *this;
        }

        [[nodiscard]] constexpr DescriptorBindingValue &setImages(const std::vector<Image *> &images) {
            this->_images = images;

            return *this;
        }

        [[nodiscard]] constexpr DescriptorBindingValue &setSamplers(std::vector<Sampler *> &&samplers) {
            this->_samplers = samplers;

            return *this;
        }

        [[nodiscard]] constexpr DescriptorBindingValue &setSamplers(const std::vector<Sampler *> &samplers) {
            this->_samplers = samplers;

            return *this;
        }

        [[nodiscard]] constexpr const std::uint32_t &getBindingIdx() const { return this->_bindingIdx; }

        [[nodiscard]] constexpr const std::vector<Buffer *> &getBuffers() const { return this->_buffers; }

        [[nodiscard]] constexpr const std::vector<Image *> &getImages() const { return this->_images; }

        [[nodiscard]] constexpr const std::vector<Sampler *> &getSamplers() const { return this->_samplers; }

        [[nodiscard]] constexpr bool operator==(const DescriptorBindingValue &) const = default;

    private:
        std::uint32_t _bindingIdx;
        std::vector<Buffer *> _buffers;
        std::vector<Image *> _images;
        std::vector<Sampler *> _samplers;
    };
}

namespace std {

    template<>
    struct hash<Penrose::DescriptorBindingValue> {
        size_t operator()(const Penrose::DescriptorBindingValue &bindingValue) const {
            return hash<std::uint32_t>{}(bindingValue.getBindingIdx())
                   ^ (Penrose::hashOf(bindingValue.getBuffers()) << 1)
                   ^ (Penrose::hashOf(bindingValue.getImages()) << 2)
                   ^ (Penrose::hashOf(bindingValue.getSamplers()) << 3);
        }
    };
}

#endif // PENROSE_RENDERING_DESCRIPTOR_BINDING_VALUE_HPP
