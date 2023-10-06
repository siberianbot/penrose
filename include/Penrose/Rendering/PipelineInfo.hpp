#ifndef PENROSE_RENDERING_PIPELINE_INFO_HPP
#define PENROSE_RENDERING_PIPELINE_INFO_HPP

#include <cstdint>
#include <functional>
#include <string>
#include <vector>

namespace Penrose {

    enum class PipelineShaderStageType {
        Vertex,
        Fragment
    };

    enum class PipelineBindingInputRate {
        Vertex,
        Instance
    };

    enum class PipelineBindingAttributeFormat {
        UInt,
        Vec2,
        Vec3,
        Mat4
    };

    enum class PipelineLayoutBindingType {
        Sampler,
        InputAttachment,
        UniformBuffer
    };

    class PipelineShaderStage {
    public:
        explicit constexpr PipelineShaderStage(PipelineShaderStageType type,
                                               std::string &&shaderAssetName)
                : _type(type),
                  _shaderAssetName(shaderAssetName) {
            //
        }

        [[nodiscard]] constexpr PipelineShaderStage &setType(PipelineShaderStageType type) {
            this->_type = type;

            return *this;
        }

        [[nodiscard]] constexpr PipelineShaderStage &setShaderAssetName(const std::string &shaderAssetName) {
            this->_shaderAssetName = shaderAssetName;

            return *this;
        }

        [[nodiscard]] constexpr const PipelineShaderStageType &getType() const { return this->_type; }

        [[nodiscard]] constexpr const std::string &getShaderAssetName() const { return this->_shaderAssetName; }

    private:
        PipelineShaderStageType _type;
        std::string _shaderAssetName;
    };

    class PipelineBindingAttribute {
    public:
        explicit constexpr PipelineBindingAttribute(PipelineBindingAttributeFormat format,
                                                    std::uint32_t offset)
                : _format(format),
                  _offset(offset) {
            //
        }

        [[nodiscard]] constexpr PipelineBindingAttribute &setFormat(PipelineBindingAttributeFormat format) {
            this->_format = format;

            return *this;
        }

        [[nodiscard]] constexpr PipelineBindingAttribute &setOffset(std::uint32_t offset) {
            this->_offset = offset;

            return *this;
        }

        [[nodiscard]] constexpr const PipelineBindingAttributeFormat &getFormat() const { return this->_format; }

        [[nodiscard]] constexpr const std::uint32_t &getOffset() const { return this->_offset; }

    private:
        PipelineBindingAttributeFormat _format;
        std::uint32_t _offset;
    };

    class PipelineBinding {
    public:
        explicit constexpr PipelineBinding(PipelineBindingInputRate inputRate,
                                           std::uint32_t stride,
                                           std::vector<PipelineBindingAttribute> &&attributes = {})
                : _inputRate(inputRate),
                  _stride(stride),
                  _attributes(attributes) {
            //
        }

        [[nodiscard]] constexpr PipelineBinding &setInputRate(PipelineBindingInputRate inputRate) {
            this->_inputRate = inputRate;

            return *this;
        }

        [[nodiscard]] constexpr PipelineBinding &setStride(std::uint32_t stride) {
            this->_stride = stride;

            return *this;
        }

        [[nodiscard]] constexpr PipelineBinding &addAttribute(const PipelineBindingAttribute &attribute) {
            this->_attributes.push_back(attribute);

            return *this;
        }

        [[nodiscard]] constexpr PipelineBinding &addAttribute(PipelineBindingAttribute &&attribute) {
            this->_attributes.push_back(attribute);

            return *this;
        }

        [[nodiscard]] constexpr const PipelineBindingInputRate &getInputRate() const { return this->_inputRate; }

        [[nodiscard]] constexpr const std::uint32_t &getStride() const { return this->_stride; }

        [[nodiscard]] constexpr const std::vector<PipelineBindingAttribute> &getAttributes() const {
            return this->_attributes;
        }

    private:
        PipelineBindingInputRate _inputRate;
        std::uint32_t _stride;
        std::vector<PipelineBindingAttribute> _attributes;
    };

    class PipelineLayoutConstant {
    public:
        explicit constexpr PipelineLayoutConstant(PipelineShaderStageType shaderStageType,
                                                  std::uint32_t offset, std::uint32_t size)
                : _shaderStageType(shaderStageType),
                  _offset(offset), _size(size) {
            //
        }

        [[nodiscard]] constexpr const PipelineShaderStageType &getShaderStageType() const {
            return this->_shaderStageType;
        }

        [[nodiscard]] constexpr const std::uint32_t &getOffset() const { return this->_offset; }

        [[nodiscard]] constexpr const std::uint32_t &getSize() const { return this->_size; }

    private:
        PipelineShaderStageType _shaderStageType;
        std::uint32_t _offset;
        std::uint32_t _size;
    };

    class PipelineLayoutBinding {
    public:
        explicit constexpr PipelineLayoutBinding(PipelineShaderStageType shaderStageType,
                                                 PipelineLayoutBindingType type, std::uint32_t count)
                : _shaderStageType(shaderStageType),
                  _type(type),
                  _count(count) {
            //
        }

        [[nodiscard]] constexpr const PipelineShaderStageType &getShaderStageType() const {
            return this->_shaderStageType;
        }

        [[nodiscard]] constexpr const PipelineLayoutBindingType &getType() const { return this->_type; }

        [[nodiscard]] constexpr const std::uint32_t &getCount() const { return this->_count; }

    private:
        PipelineShaderStageType _shaderStageType;
        PipelineLayoutBindingType _type;
        std::uint32_t _count;
    };

    class PipelineLayout {
    public:
        explicit constexpr PipelineLayout(std::vector<PipelineLayoutConstant> &&constants = {},
                                          std::vector<PipelineLayoutBinding> &&bindings = {})
                : _constants(constants),
                  _bindings(bindings) {
            //
        }

        [[nodiscard]] constexpr PipelineLayout &addConstant(const PipelineLayoutConstant &constant) {
            this->_constants.push_back(constant);

            return *this;
        }

        [[nodiscard]] constexpr PipelineLayout &addConstant(PipelineLayoutConstant &&constant) {
            this->_constants.push_back(constant);

            return *this;
        }

        [[nodiscard]] constexpr PipelineLayout &addBinding(const PipelineLayoutBinding &binding) {
            this->_bindings.push_back(binding);

            return *this;
        }

        [[nodiscard]] constexpr PipelineLayout &addBinding(PipelineLayoutBinding &&binding) {
            this->_bindings.push_back(binding);

            return *this;
        }

        [[nodiscard]] constexpr const std::vector<PipelineLayoutConstant> &getConstants() const {
            return this->_constants;
        }

        [[nodiscard]] constexpr const std::vector<PipelineLayoutBinding> &getBindings() const {
            return this->_bindings;
        }

    private:
        std::vector<PipelineLayoutConstant> _constants;
        std::vector<PipelineLayoutBinding> _bindings;
    };

    class PipelineInfo {
    public:
        explicit constexpr PipelineInfo(PipelineLayout &&layout = PipelineLayout{},
                                        std::vector<PipelineShaderStage> &&stages = {},
                                        std::vector<PipelineBinding> &&bindings = {})
                : _layout(layout),
                  _stages(stages),
                  _bindings(bindings) {
            //
        }

        [[nodiscard]] constexpr PipelineInfo &setLayout(const PipelineLayout &layout) {
            this->_layout = layout;

            return *this;
        }

        [[nodiscard]] constexpr PipelineInfo &setLayout(PipelineLayout &&layout) {
            this->_layout = layout;

            return *this;
        }

        [[nodiscard]] constexpr PipelineInfo &addStage(const PipelineShaderStage &stage) {
            this->_stages.push_back(stage);

            return *this;
        }

        [[nodiscard]] constexpr PipelineInfo &addStage(PipelineShaderStage &&stage) {
            this->_stages.push_back(stage);

            return *this;
        }

        [[nodiscard]] constexpr PipelineInfo &addBinding(const PipelineBinding &binding) {
            this->_bindings.push_back(binding);

            return *this;
        }

        [[nodiscard]] constexpr PipelineInfo &addBinding(PipelineBinding &&binding) {
            this->_bindings.push_back(binding);

            return *this;
        }

        [[nodiscard]] constexpr const PipelineLayout &getLayout() const { return this->_layout; }

        [[nodiscard]] constexpr const std::vector<PipelineShaderStage> &getStages() const { return this->_stages; }

        [[nodiscard]] constexpr const std::vector<PipelineBinding> &getBindings() const { return this->_bindings; }

    private:
        PipelineLayout _layout;
        std::vector<PipelineShaderStage> _stages;
        std::vector<PipelineBinding> _bindings;
    };
}

namespace std {

    template<>
    struct hash<Penrose::PipelineShaderStage> {
        size_t operator()(const Penrose::PipelineShaderStage &shaderStage) const {
            return hash<string>{}(shaderStage.getShaderAssetName())
                   ^ (hash<Penrose::PipelineShaderStageType>{}(shaderStage.getType()) << 1);
        }
    };

    template<>
    struct hash<Penrose::PipelineBindingAttribute> {
        size_t operator()(const Penrose::PipelineBindingAttribute &bindingAttribute) const {
            return hash<Penrose::PipelineBindingAttributeFormat>{}(bindingAttribute.getFormat())
                   ^ (hash<uint32_t>{}(bindingAttribute.getOffset()) << 1);
        }
    };

    template<>
    struct hash<Penrose::PipelineBinding> {
        size_t operator()(const Penrose::PipelineBinding &binding) const {
            auto h = hash<uint32_t>{}(binding.getStride())
                     ^ (hash<Penrose::PipelineBindingInputRate>{}(binding.getInputRate()) << 1);

            for (std::uint32_t idx = 0; idx < binding.getAttributes().size(); idx++) {
                h ^= (hash<Penrose::PipelineBindingAttribute>{}(binding.getAttributes().at(idx)) << (idx + 2));
            }

            return h;
        }
    };

    template<>
    struct hash<Penrose::PipelineLayoutConstant> {
        size_t operator()(const Penrose::PipelineLayoutConstant &constant) const {
            return hash<std::uint32_t>{}(constant.getSize())
                   ^ (hash<std::uint32_t>{}(constant.getOffset()) << 1)
                   ^ (hash<Penrose::PipelineShaderStageType>{}(constant.getShaderStageType()) << 2);
        }
    };

    template<>
    struct hash<Penrose::PipelineLayoutBinding> {
        size_t operator()(const Penrose::PipelineLayoutBinding &binding) const {
            return hash<std::uint32_t>{}(binding.getCount())
                   ^ (hash<Penrose::PipelineLayoutBindingType>{}(binding.getType()) << 1)
                   ^ (hash<Penrose::PipelineShaderStageType>{}(binding.getShaderStageType()) << 2);
        }
    };

    template<>
    struct hash<Penrose::PipelineLayout> {
        size_t operator()(const Penrose::PipelineLayout &layout) const {
            size_t h = 0x9e3779b9;

            for (std::uint32_t idx = 0; idx < layout.getBindings().size(); idx++) {
                h ^= (hash<Penrose::PipelineLayoutBinding>{}(layout.getBindings().at(idx)) << idx);
            }

            for (std::uint32_t idx = 0; idx < layout.getConstants().size(); idx++) {
                h ^= (hash<Penrose::PipelineLayoutConstant>{}(layout.getConstants().at(idx))
                        << (idx + layout.getBindings().size()));
            }

            return h;
        }
    };

    template<>
    struct hash<Penrose::PipelineInfo> {
        size_t operator()(const Penrose::PipelineInfo &pipelineInfo) const {
            size_t h = hash<Penrose::PipelineLayout>{}(pipelineInfo.getLayout());

            for (std::uint32_t idx = 0; idx < pipelineInfo.getBindings().size(); idx++) {
                h ^= (hash<Penrose::PipelineBinding>{}(pipelineInfo.getBindings().at(idx)) << (idx + 1));
            }

            for (std::uint32_t idx = 0; idx < pipelineInfo.getStages().size(); idx++) {
                h ^= (hash<Penrose::PipelineShaderStage>{}(pipelineInfo.getStages().at(idx))
                        << (idx + pipelineInfo.getBindings().size() + 1));
            }

            return h;
        }
    };
}

#endif // PENROSE_RENDERING_PIPELINE_INFO_HPP
