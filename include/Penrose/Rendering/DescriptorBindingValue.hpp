#ifndef PENROSE_RENDERING_DESCRIPTOR_BINDING_VALUE_HPP
#define PENROSE_RENDERING_DESCRIPTOR_BINDING_VALUE_HPP

#include <cstdint>
#include <optional>

namespace Penrose {

    class Buffer;
    class Image;
    class Sampler;

    class DescriptorBindingValue {
    public:
        explicit constexpr DescriptorBindingValue(std::uint32_t bindingIdx,
                                                  std::optional<Buffer *> buffer = std::nullopt,
                                                  std::optional<Image *> image = std::nullopt,
                                                  std::optional<Sampler *> sampler = std::nullopt)
                : _bindingIdx(bindingIdx),
                  _buffer(buffer),
                  _image(image),
                  _sampler(sampler) {
            //
        }

        [[nodiscard]] constexpr DescriptorBindingValue &setBuffer(std::optional<Buffer *> &&buffer) {
            this->_buffer = buffer;

            return *this;
        }

        [[nodiscard]] constexpr DescriptorBindingValue &setBuffer(const std::optional<Buffer *> &buffer) {
            this->_buffer = buffer;

            return *this;
        }

        [[nodiscard]] constexpr DescriptorBindingValue &setImage(std::optional<Image *> &&image) {
            this->_image = image;

            return *this;
        }

        [[nodiscard]] constexpr DescriptorBindingValue &setImage(const std::optional<Image *> &image) {
            this->_image = image;

            return *this;
        }

        [[nodiscard]] constexpr DescriptorBindingValue &setSampler(std::optional<Sampler *> &&sampler) {
            this->_sampler = sampler;

            return *this;
        }

        [[nodiscard]] constexpr DescriptorBindingValue &setSampler(const std::optional<Sampler *> &sampler) {
            this->_sampler = sampler;

            return *this;
        }

        [[nodiscard]] constexpr const std::uint32_t &getBindingIdx() const { return this->_bindingIdx; }

        [[nodiscard]] constexpr const std::optional<Buffer *> &getBuffer() const { return this->_buffer; }

        [[nodiscard]] constexpr const std::optional<Image *> &getImage() const { return this->_image; }

        [[nodiscard]] constexpr const std::optional<Sampler *> &getSampler() const { return this->_sampler; }

        [[nodiscard]] constexpr bool operator==(const DescriptorBindingValue &) const = default;

    private:
        std::uint32_t _bindingIdx;
        std::optional<Buffer *> _buffer;
        std::optional<Image *> _image;
        std::optional<Sampler *> _sampler;
    };
}

namespace std {

    template<>
    struct hash<Penrose::DescriptorBindingValue> {
        size_t operator()(const Penrose::DescriptorBindingValue &bindingValue) const {
            return hash<std::uint32_t>{}(bindingValue.getBindingIdx())
                   ^ (hash<std::optional<Penrose::Buffer *>>{}(bindingValue.getBuffer()) << 1)
                   ^ (hash<std::optional<Penrose::Image *>>{}(bindingValue.getImage()) << 2)
                   ^ (hash<std::optional<Penrose::Sampler *>>{}(bindingValue.getSampler()) << 3);
        }
    };
}

#endif // PENROSE_RENDERING_DESCRIPTOR_BINDING_VALUE_HPP
