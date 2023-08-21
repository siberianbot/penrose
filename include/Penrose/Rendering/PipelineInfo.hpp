#ifndef PENROSE_RENDERING_PIPELINE_INFO_HPP
#define PENROSE_RENDERING_PIPELINE_INFO_HPP

#include <cstdint>
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
        Vec2,
        Vec3
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

    class PipelineInfo {
    public:
        explicit constexpr PipelineInfo(std::vector<PipelineShaderStage> &&stages = {},
                                        std::vector<PipelineBinding> &&bindings = {})
                : _stages(stages),
                  _bindings(bindings) {
            //
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

        [[nodiscard]] constexpr const std::vector<PipelineShaderStage> &getStages() const { return this->_stages; }

        [[nodiscard]] constexpr const std::vector<PipelineBinding> &getBindings() const { return this->_bindings; }

    private:
        std::vector<PipelineShaderStage> _stages;
        std::vector<PipelineBinding> _bindings;
    };
}

#endif // PENROSE_RENDERING_PIPELINE_INFO_HPP
