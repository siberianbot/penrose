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

    class PipelineShaderStage {
    public:
        [[nodiscard]] PipelineShaderStage &setType(PipelineShaderStageType type);
        [[nodiscard]] PipelineShaderStage &setShaderAssetName(const std::string &shaderAssetName);

        [[nodiscard]] const PipelineShaderStageType &getType() const { return this->_type; }

        [[nodiscard]] const std::string &getShaderAssetName() const { return this->_shaderAssetName; }

    private:
        PipelineShaderStageType _type;
        std::string _shaderAssetName;
    };

    enum class PipelineBindingInputRate {
        Vertex,
        Instance
    };

    enum class PipelineBindingAttributeFormat {
        Vec2,
        Vec3
    };

    class PipelineBindingAttribute {
    public:
        [[nodiscard]] PipelineBindingAttribute &setFormat(PipelineBindingAttributeFormat format);
        [[nodiscard]] PipelineBindingAttribute &setOffset(std::uint32_t offset);

        [[nodiscard]] const PipelineBindingAttributeFormat &getFormat() const { return this->_format; }

        [[nodiscard]] const std::uint32_t &getOffset() const { return this->_offset; }

    private:
        PipelineBindingAttributeFormat _format;
        std::uint32_t _offset;
    };

    class PipelineBinding {
    public:
        [[nodiscard]] PipelineBinding &setInputRate(PipelineBindingInputRate inputRate);
        [[nodiscard]] PipelineBinding &setStride(std::uint32_t stride);
        [[nodiscard]] PipelineBinding &addAttribute(PipelineBindingAttribute &&attribute);

        [[nodiscard]] const PipelineBindingInputRate &getInputRate() const { return this->_inputRate; }

        [[nodiscard]] const std::uint32_t &getStride() const { return this->_stride; }

        [[nodiscard]] const std::vector<PipelineBindingAttribute> &getAttributes() const { return this->_attributes; }

    private:
        PipelineBindingInputRate _inputRate;
        std::uint32_t _stride;
        std::vector<PipelineBindingAttribute> _attributes;
    };

    class PipelineInfo {
    public:
        [[nodiscard]] PipelineInfo &addStage(PipelineShaderStage &&stage);
        [[nodiscard]] PipelineInfo &addBinding(PipelineBinding &&binding);

        [[nodiscard]] const std::vector<PipelineShaderStage> &getStages() const { return this->_stages; }

        [[nodiscard]] const std::vector<PipelineBinding> &getBindings() const { return this->_bindings; }

    private:
        std::vector<PipelineShaderStage> _stages;
        std::vector<PipelineBinding> _bindings;
    };
}

#endif // PENROSE_RENDERING_PIPELINE_INFO_HPP
